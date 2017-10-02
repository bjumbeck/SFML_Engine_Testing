#include "Entity.hpp"
#include "EntityManager.hpp"

const Entity::Id Entity::INVALID_ID;

Entity::Entity()
    : id(INVALID_ID)
    , entityManager(nullptr)
{}

Entity::Entity(EntityManager* manager, Entity::Id paramId)
    : id(paramId)
    , entityManager(manager)
{}

bool Entity::valid() const
{
    return entityManager && entityManager->validEntity(id);
}