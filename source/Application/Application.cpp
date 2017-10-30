#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include "Application.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Entity/Entity.hpp"
#include "Components/Component.hpp"

#include "Systems/RenderSystem.hpp"
#include "Systems/MovementSystem.hpp"

#include "Components/TransformableComponent.hpp"
#include "Components/RenderableComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/SteeringComponent.hpp"

#include "EventManagement/EventManager.hpp"
#include "Entity/EntityManager.hpp"
#include "Systems/SystemManager.hpp"

const sf::Time Application::timePerFrame = sf::seconds(1.0f / 60.0f);

Application::~Application() = default;

Application::Application()
    : window(sf::VideoMode(500, 500), "Testing Grounds", sf::Style::Close)
    , eventManager(std::make_unique<EventManager>())
    , entityManager(std::make_unique<EntityManager>(*eventManager))
    , systemManager(std::make_unique<SystemManager>(*entityManager, *eventManager))
{
    // Testing / Feature Demo code.

    // Enities represent primarily just a unique ID and that is it.
    // All components are housed in their own seperate memory pools 
    // (Based on component families) inside of EntityManager.
    Entity playerTest = entityManager->createEntity();

    // Test 1: Rendering - Adding the Transformable and Renderable components to an entity will 
    // make it be drawn.
    g_TextureManager.loadResource("TestSprite.png", "Assets/Textures/TestSprite.png");
    sf::Texture* texture = g_TextureManager.getResource("TestSprite.png");
    sf::Vector2f origin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
    entityManager->assignComponent<TransformableComponent>(playerTest.id(), sf::Vector2f(100.0f, 100.0f), 0, sf::Vector2f(1.0f, 1.0f), origin);
    entityManager->assignComponent<RenderableComponent>(playerTest.id(), "TestSprite.png");

    // Test 2: Movement - Adding the Movement component to an entity will allow for either Dynamic Movement either through
    // an AI controller or player controller.
    // Params: Mass, Max Velocity, Max Force, Max Turn Rate
    entityManager->assignComponent<MovementComponent>(playerTest.id(), 1.0f, 50.0f, 10.0f, 10.0f);

    // Test 3: Base AI steering: Steering Behaviors - Adding the Steering component will allow you to use 
    // steering behaviors to move an entity around with dynamic movement.
    ComponentPtr<SteeringComponent> steering = entityManager->assignComponent<SteeringComponent>(playerTest.id());
    steering->behaviorFlags |= BehaviorType::Arrive;
    steering->arrivePosition = sf::Vector2f(400, 400);
    steering->arriveDeceleration = SteeringComponent::Deceleration::Normal;

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
    systemManager->addSystem<RenderSystem>(window);
    systemManager->addSystem<MovementSystem>();

    systemManager->configure();
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
    systemManager->updateAllSystems(deltaTime);
}

void Application::renderFrame()
{
    window.clear(sf::Color::Black);
    systemManager->getSystem<RenderSystem>()->render(*entityManager);
    window.display();
}