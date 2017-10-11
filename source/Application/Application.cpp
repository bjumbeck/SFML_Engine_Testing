#include <SFML/Window/Event.hpp>

#include "Application.hpp"
#include "Systems/RenderSystem.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Entity/Entity.hpp"
#include "Components/Component.hpp"
#include "Components/TransformableComponent.hpp"
#include "Components/RenderableComponent.hpp"

const sf::Time Application::timePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application()
    : window(sf::VideoMode(500, 500), "Testing Grounds", sf::Style::Close)
    , eventManager()
    , entityManager(eventManager)
    , systemManager(entityManager, eventManager)
{
    // Testing code
    g_TextureManager.loadResource("Assets/Textures/TestImage.png", "TestImage.png");

    Entity playerTest = entityManager.createEntity();

    entityManager.assignComponent<TransformableComponent>(playerTest.getId(), sf::Vector2f(100.0f, 100.0f));
    entityManager.assignComponent<RenderableComponent>(playerTest.getId(), "TestImage.png");

    setupSystems();
}

void Application::runApplication()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;

        while (timeSinceLastUpdate >= timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            processSFMLEvents();
            updateFrame(timePerFrame);
        }

        renderFrame();
    }
}

void Application::setupSystems()
{
    systemManager.addSystem<RenderSystem>(window);

    systemManager.configure();
}

void Application::processSFMLEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

void Application::updateFrame(const sf::Time& deltaTime)
{
    systemManager.updateAllSystems(deltaTime);
}

void Application::renderFrame()
{
    window.clear(sf::Color::Black);
    systemManager.getSystem<RenderSystem>()->render(entityManager);
    window.display();
}