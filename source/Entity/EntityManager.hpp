#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <cstdint>
#include <vector>
#include <bitset>
#include <iterator>
#include <algorithm>

#include "Helpers/MemoryPool.hpp"
#include "Entity.hpp"

class EventManager;

const int MAX_COMPONENTS = 64;

class EntityManager : private sf::NonCopyable
{
    public:
        using ComponentMask = std::bitset<MAX_COMPONENTS>;

    public:
        explicit EntityManager(EventManager& eventManager);
        ~EntityManager();

        // Entity Management
        Entity createEntity();
        Entity::Id createEntityId(uint32_t index) const;
        void destroyEntity(Entity::Id entityId);
        Entity getEntity(Entity::Id entityId);
        bool validEntity(Entity::Id id) const;

        // Entity Component Management
        template <typename CompType, typename ... Args>
        ComponentPtr<CompType> assignComponent(Entity::Id id, Args&& ... args)
        {
            assertValidId(id);

            const BaseComponent::Family family = componentFamily<CompType>();
            assert(!entityComponentMasks[id.index()].test(family));

            // Add it into a memory pool for the component family
            Pool<CompType>* pool = accomodateComponent<CompType>();
            new(pool->get(id.index())) C(std::forward<Args>(args) ...);

            // Set the bit for the component
            entityComponentMasks[id.index()].set(family);

            // Create the component
            ComponentPtr<CompType> component(this, id);
            eventManager.emit<ComponentAddedEvent<CompType>>(Entity(this, id), component);

            return component;
        }

        template <typename CompType>
        void removeComponent(Entity::Id id)
        {
            assertValidId(id);

            const BaseComponent::Family family = componentFamily<CompType>();
            const uint32_t index = id.index();

            BasePool* pool = componentPools[family];
            ComponentPtr<CompType> component(this, id);
            eventManager.emit<ComponentRemovedEvent<CompType>>(Entity(this, id), component);

            entityComponentMasks[id.index()].reset(family);
            pool->destroy(index);
        }

        template <typename CompType>
        bool hasComponent(Entity::Id id) const
        {
            assertValidId(id);

            const BaseComponent::Family family = componentFamily<CompType>();
            if (family >= componentPools.size())
            {
                return false;
            }

            BasePool* pool = componentPools[family];
            if (!pool || !entityComponentMasks[id.index()][family])
            {
                return false;
            }

            return true;
        }

        template <typename CompType, typename std::enable_if_t<!std::is_const<CompType>::value>
        ComponentPtr<CompType> getComponent(Entity::Id id)
        {
            assertValidId(id);

            const BaseComponent::Family family = componentFamily<CompType>();
            if (family >= componentPools.size())
            {
                return ComponentPtr<CompType>();
            }

            BasePool* pool = componentPools[family];
            if (!pool || !entityComponentMasks[id.index()][family])
            {
                return ComponentPtr<CompType>()
            }

            return ComponentPtr<CompType>(this, id);
        }

        // Container Management
        size_t size() const { return entityComponentMasks.size() - freeIds.size(); }
        size_t capacity() const { return entityComponentMasks.size(); }

    private:
        friend class Entity;

        template <typename CompType, typename EManager>
        friend class ComponentPtr;

        uint32_t indexCounter = 0;

        EventManager& eventManager;

        std::vector<BasePool*> componentPools;
        std::vector<ComponentMask> entityComponentMasks;
        std::vector<uint32_t> entityVersions;
        std::vector<uint32_t> freeIds;
};


/////////////////////////////////////////////////
// Helper convience classes for EntityManager
/////////////////////////////////////////////////

// An iterator over the entities in EntityManager (Through the views below)
// If All is true then it will iterate over all entities and  
// ignore entity masks.
template<class Delegate, bool All = false>
class ViewIterator : public std::iterator<std::input_iterator_tag, Entity::Id>
{
    public:
        Delegate& operator++()
        {
            ++idIndex;
            next();

            return *static_cast<Delegate*>(this);
        }

        bool operator==(const Delegate& rhs) const { return idIndex == rhs.idIndex; }
        bool operator!=(const Delegate& rhs) const { return idIndex != rhs.idIndex; }
        Entity operator*() { return Entity(entityManager, entityManager->createId(idIndex)); }
        const Entity operator*() const { return Entity(entityManager, entityManager->createId(idIndex)); }

    protected:
        ViewIterator(EntityManager* manager, uint32_t index)
            : entityManager(manager)
            , idIndex(index)
            , capacity(entityManager->capacity())
            , freeCursor(~0UL)
        {
            if (All)
            {
                std::sort(entityManager->freeIds.begin(), entityManager.freeIds.end());
                freeCursor = 0;
            }
        }

        ViewIterator(EntityManager* manager, const ComponentMask mask, uint32_t index)
            : entityManager(manager)
            , compMask(mask)
            , idIndex(index)
            , capacity(entityManager->capacity())
            , freeCursor(~0UL)
        {
            if (All)
            {
                std::sort(entityManager->freeIds.begin(), entityManager->freeIds.end());
                freeCursor = 0;
            }
        }

        void next()
        {
            while (idIndex < capacity && !predicate())
            {
                ++idIndex;
            }

            if (idIndex < capacity)
            {
                Entity entity = entityManager->getEntity(entityManager->createId(idIndex));
                static_cast<Delegate*>(this)->nextEntity(entity);
            }
        }

        inline bool predicate() const
        {
            return All && validEntity() || 
                   (entityManager->entityComponentMasks[idIndex] & mask) == compMask;
        }

        inline bool validEntity() const
        {
            const std::vector<uint32_t>& freeVec = entityManager->freeIds;
            if (freeCursor < freeVec.size() && freeVec[freeCursor] == idIndex)
            {
                ++freeCursor;

                return false;
            }

            return true;
        }

    public:
        EntityManager* entityManager;
        ComponentMask compMask;
        uint32_t idIndex;
        size_t capacity;
        size_t freeCursor;
};

template <bool All>
class BaseView
{
    public:
        class Iterator : public ViewIterator<Iterator, All>
        {
            public:
                Iterator(EntityManager* manager,
                         const ComponentMask mask,
                         uint32_t index)
                    : ViewIterator<Iterator, All>(manager, mask, index)
                {
                    ViewIterator<Iterator, All>::next();
                }

                void nextEntity(Entity& entity) {}
        };

        Iterator begin() { return Iterator(entityManager, compMask, 0); }
        Iterator end() { return Iterator(entityManager, compMask, static_cast<uint32_t>(entityManager->capacity())); }
        const Iterator begin() const { return Iterator(entityManager, compMask, 0); }
        const Iterator end() const { return Iterator(entityManager, compMask, entityManager->capacity()); }

    private:
        friend class EntityManager;

        explicit BaseView(EntityManager* manager)
            : (entityManager)
        {
            compMask.set();
        }

        BaseView(EntityManager* manager, ComponentMask mask)
            : entityManager(manager)
            , compMask(mask)
        {}

    private:
        EntityManager* entityManager;
        ComponentMask compMask;
};

// Defines for the different types of views
using NormalView = BaseView<false>;
using DebugView = BaseView<true>;

template <typename ... Components>
class UnpackingView
{
    public:
        class Unpacker
        {
            public:
                explicit Unpacker(ComponentPtr<Components>& ... ptrs)
                    : compPtrs(std::tuple<ComponentPtr<Components>& ...>(ptrs...))
                {}

                void unpack(Entity& entity) const
                {
                    unpackImpl<0, Components...>(entity);
                }

            private:
                template <int N, typename CompType>
                void unpackImpl(Entity& entity) const
                {
                    std::get<N>(compPtrs) = getComponent<CompType>(entity.getId);
                }

                template <int N, typename Comp1, typename Comp2, typename ... CompN>
                void unpackImpl(Entity& entity) const
                {
                    std::get<N>(compPtrs) = getComponent<Comp1>();
                    unpackImpl<N + 1, Comp2, CompN...>(entity);
                }

            private:
                std::tuple<ComponentPtr<Components>& ...> compPtrs;
        };

        class Iterator : public ViewIterator<Iterator>
        {
            public:
                Iterator(EntityManager* manager,
                         const ComponentMask mask,
                         uint32_t index,
                         const Unpacker& unpacker)
                    : ViewIterator<Iterator>(manager, mask, index)
                    , unpacker(unpacker)
                {
                    ViewIterator<Iterator>::next();
                }

                void nextEntity(Entity& entity)
                {
                    unpacker.unpack(entity);
                }

            private:
                const Unpacker& unpacker;
        };

    public:
        Iterator begin() { return Iterator(entityManager, compMask, 0, unpacker); }
        Iterator end() { return Iterator(entityManager, compMask, entityManager->capacity(), unpacker); }
        const Iterator begin() const { return Iterator(entityManager, compMask, 0, unpacker); }
        const Iterator end() const { return Iterator(entityManager, compMask, entityManager->capacity(), unpacker); }

    private:
        UnpackingView(EntityManager* manager, ComponentMask mask, ComponentPtr<Components>& ... ptrs)
            : entityManager(manager)
            , compMask(mask)
            , unpacker(ptrs...)
        {}

    private:
        friend class EntityManager;

        EntityManager* manager;
        ComponentMask compMask;
        Unpacker unpacker;
};