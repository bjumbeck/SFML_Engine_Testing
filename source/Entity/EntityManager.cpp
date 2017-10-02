#include "EntityManager.hpp"

bool EntityManager::validEntity(Entity::Id id) const
{
    return id.index() < entityVersions.size() &&
           entityVersions[id.index()] == id.version();
}