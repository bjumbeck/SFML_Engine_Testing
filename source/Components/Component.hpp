#pragma once

#include "Entity/Entity.hpp"

template <typename CompType, typename EManager>
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