#pragma once

#include "EventManagement/EventManager.hpp"
#include "Entity/Entity.hpp"
#include "Components/Component.hpp"

template <typename CompType>
struct ComponentAddedEvent : public Event<ComponentAddedEvent<CompType>>
{
    ComponentAddedEvent(Entity entity, ComponentPtr<CompType> component)
        : entity(entity)
        , component(component)
    {}

    Entity entity;
    ComponentPtr<CompType> component;
};

template <typename CompType>
struct ComponentRemovedEvent : public Event<ComponentRemovedEvent<CompType>>
{
    ComponentRemovedEvent(Entity entity, ComponentPtr<CompType> component)
        : entity(entity)
        , component(component)
    {}

    Entity entity;
    ComponentPtr<CompType> component;
};