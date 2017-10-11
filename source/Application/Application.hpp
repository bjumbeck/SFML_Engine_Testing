#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "EventManagement/EventManager.hpp"
#include "Entity/EntityManager.hpp"
#include "Systems/SystemManager.hpp"

class Application : private sf::NonCopyable
{
    public:
        static Application& getInstance() { static Application instance; return instance; }
        Application(const Application&) = delete;
        void operator=(const Application&) = delete;
        void runApplication();

        const sf::RenderWindow& getWindow() const { return window; } 

    private:
        Application();

        void setupSystems();
        
        void processSFMLEvents();
        void updateFrame(const sf::Time& deltaTime);
        void renderFrame();

    private:
        static const sf::Time timePerFrame;

        sf::RenderWindow window;

        // Do not change the ordering of these
        // will mess up the constructor if you do.
        EventManager eventManager;
        EntityManager entityManager;
        SystemManager systemManager;
};