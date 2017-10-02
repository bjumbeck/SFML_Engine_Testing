#pragma once

#include <cstdint>

class EntityManager;

class Entity
{
    public:
        // Id implementation inspired by the entityX framework, all credit of 
        // the representation of Id goes to them.
        struct Id
        {
            Id() : internalId(0) {}
            explicit Id(std::uint64_t id) : internalId(id) {}
            Id(std::uint32_t idIndex, std::uint32_t idVersion)
                : internalId(static_cast<std::uint64_t>(idIndex) | static_cast<std::uint64_t>(idVersion) << 32UL) {}

            std::uint64_t getId() const { return internalId; }
            
            bool operator==(const Id& other) const { return internalId == other.internalId; }
            bool operator!=(const Id& other) const { return internalId != other.internalId; }
            bool operator<(const Id& other) const { return internalId < other.internalId; }

            std::uint32_t index() const { return internalId & 0xffffffffUL; }
            std::uint32_t version() const { return internalId >> 32; }

            private:
                std::uint64_t internalId;
        };

    public:
        static const Entity::Id INVALID_ID;

        // Initialization
        Entity();
        Entity(EntityManager* manager, Entity::Id paramId);
        Entity(const Entity& other) = default;
        Entity& operator=(const Entity& other) = default;

        // Entity Management, these are conveience methods
        // the functionality resides in the entity manager
        // which handles everything entity related.
        bool valid() const;
        Entity::Id getId() const { return id; }

        // Convience operator overloads
        operator bool() const { return valid(); }
        bool operator==(const Entity& other) const { return other.id == id; }
        bool operator!=(const Entity& other) const { return other.id != id; }
        bool operator<(const Entity& other) const { return other.id < id; }

    private:
        Entity::Id id;
        EntityManager* entityManager;
};