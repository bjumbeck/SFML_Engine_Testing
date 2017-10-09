#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <cstddef>

class SystemManager;
class EventManager;
class EntityManager;

class BaseSystem : private sf::NonCopyable
{
    public:
        virtual ~BaseSystem() {}

        virtual void configure(EventManager& eventManager) = 0;
        virtual void update(EntityManager& entityManager, EventManager& eventManager, const sf::Time& deltaTime) = 0;

    public:
        static std::size_t familyCounter;
};

template <typename Derived>
class System : public BaseSystem
{
    public:
        virtual ~System() {}

    private:
        static std::size_t family() { static std::size_t family = familyCounter++; return family; }

    private:
        friend class SystemManager;
};