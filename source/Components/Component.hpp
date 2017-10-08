#pragma once

#include <memory>
#include <cstdlib>

#include "Entity/Entity.hpp"

class EntityManager;

// Internal class, do not use this to define components
struct BaseComponent
{
    public:
        using Family = std::size_t;

        // Using my own memory pools to manage memory for
        // components.
        void operator delete(void* ptr) { fail(); }
        void operator delete[](void* ptr) { fail(); }

    private:
        static void fail()
        {
            #if defined(_HAS_EXCEPTIONS) || defined(_EXCEPTIONS)
                throw std::bad_alloc();
            #else
                std::abort();
            #endif
        }


    protected:
        static std::size_t familyCounter;
}

// User facing class, if you are defining a new component
// use this class to derive from.
template <typename Derived>
struct Component : public BaseComponent
{
    public:
        using Ptr = ComponentPtr<Derived>;
        using ConstPtr = ComponentPtr<const Derived, const EntityManager>;

    private:
        static Family family();

    private:
        friend class EntityManager;
}

template <typename CompType, class EManager = EntityManager>
class ComponentPtr
{
    public:
        ComponentPtr() : entityManager(nullptr) {}

        bool valid() const;
        void remove();

        CompType* get();
        const CompType* get() const;

        operator bool() const;

        CompType* operator->();
        const CompType* operator->() const;

        bool operator==(const ComponentPtr<CompType>& rhs) const;
        bool operator!=(const ComponentPtr<CompType>& rhs) const;

    private:
        ComponentPtr(EManager* manager, Entity::Id id)
            : entityManager(manager)
            , owningEntityId(id)
        {}

    private:
        friend class EntityManager;

        EManager* entityManager;
        Entity::Id owningEntityId;
}