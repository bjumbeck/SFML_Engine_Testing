#pragma once

#include <SFML/System/Vector2.hpp>

struct TransformableComponent
{
    TransformableComponent()
        : position(0.0f, 0.0f)
        , scale(1.0f, 1.0f)
        , rotation(0.0f)
    {}

    TransformableComponent(const sf::Vector2f& position,
                           const float rotation = 0.0f,
                           const sf::Vector2f& scale = sf::Vector2f(1.0f, 1.0f))
        : position(position)
        , rotation(rotation)
        , scale(scale)
    {}

    // TODO: Replace this with sf::Transform
    sf::Vector2f position;
    sf::Vector2f scale;
    float rotation;
};