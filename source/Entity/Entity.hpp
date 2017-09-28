#pragma once

#include <cstdint>

template <typename ComponentType>
class ComponentPtr;

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

            constexpr std::uint64_t getId() const { return internalId; }
            
            constexpr bool operator==(const Id& other) const { return internalId == other.internalId; }
            constexpr bool operator!=(const Id& other) const { return internalId != other.internalId; }
            constexpr bool operator<(const Id& other) const { return internalId < other.internalId; }

            constexpr std::uint32_t index() const { return internalId & 0xffffffffUL; }
            constexpr std::uint32_t version() const { return internalId >> 32; }

            private:
                std::uint64_t internalId;
        };

    public:
        static const Entity::Id InvalidId;

        // Initialization
        Entity() = default;
        Entity(Entity::Id paramId) : id(paramId) {}
        Entity(const Entity& other) = default;
        Entity& operator=(const Entity& other) = default;

        // Component Management
        template <typename CompType, typename ... Args>
        ComponentPtr<CompType> assignComponent(Args&& ... args);

        template <typename CompType, typename ... Args>
        ComponentPtr<CompType> replaceComponent(Args&& ... args);

        template <typename CompType>
        void removeComponent();

        template <typename CompType, typename = std::enable_if_t<!std::is_const<CompType>::value>>
        ComponentPtr<CompType> getComponent();

        template <typename CompType, typename = std::enable_if_t<std::is_const<CompType>::value>>
        constexpr ComponentPtr<CompType, const EntityManager> getComponent() const;

        template <typename ... Components>
        std::tuple<ComponentPtr<Components>...> getComponents();


        // Getters/Setters and operators
        bool valid() const;
        void invalidate();
        constexpr Entity::Id getId() const { return id; }

        constexpr operator bool() const { return valid(); }
        constexpr bool operator==(const Entity& other) const { return other.id == id; }
        constexpr bool operator!=(const Entity& other) const { return other.id != id; }
        constexpr bool operator<(const Entity& other) const { return other.id < id; }

    private:
        Entity::Id id = InvalidId;
}