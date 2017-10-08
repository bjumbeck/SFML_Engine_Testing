#pragma once

#include "EventManagement/EventManager.hpp"
#include "Entity/Entity.hpp"

struct EntityCreatedEvent : public Event<EntityCreatedEvent>
{
    explicit EntityCreatedEvent(Entity entity) : entity(entity) {}
    virtual ~EntityCreatedEvent() {}

    Entity entity;
};

struct EntityDestroyedEvent : public Event<EntityDestroyedEvent>
{
    explicit EntityDestroyedEvent(Entity entity) : entity(entity) {}
    virtual ~EntityDestroyedEvent() {}

    Entity entity;
};