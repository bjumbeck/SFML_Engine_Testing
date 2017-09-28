#include <SFML/Window/Event.hpp>

#include "Application.hpp"

const sf::Time Application::timePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application()
    : window(sf::VideoMode(500, 500), "Testing Grounds", sf::Style::Close)
{

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

}

void Application::renderFrame()
{
    window.clear(sf::Color::Black);
    
    window.display();
}