#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <unordered_map>
#include <memory>

class BaseSystem;
class EventManager;
class EntityManager;

class SystemManager : private sf::NonCopyable
{
    public:
        SystemManager(EntityManager& entityManager, EventManager& eventManager)
            : isInitialized(false)
            , entityManager(entityManager)
            , eventManager(eventManager)
        {}

        template <typename SystemType>
        void addSystem(std::shared_ptr<SystemType> system);

        template <typename SystemType, typename ... Args>
        std::shared_ptr<SystemType> addSystem(Args&& ... args);

        template <typename SystemType>
        std::shared_ptr<SystemType> getSystem();

        template <typename SystemType>
        void update(const sf::Time& deltaTime);
        
        void updateAllSystems(const sf::Time& deltaTime);

        void configure();

    private:
        bool isInitialized;
        EntityManager& entityManager;
        EventManager& eventManager;
        std::unordered_map<std::size_t, std::shared_ptr<BaseSystem>> systems;
};

#include "SystemManager.inl"