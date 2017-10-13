#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include "ResourceManagement/ResourceManager.hpp"

struct RenderableComponent
{
    RenderableComponent(const std::string& textureFileName);
    RenderableComponent(const std::string& textureFileName, const sf::FloatRect& textureRect);

    sf::VertexArray vertexArray;
    sf::Texture* texture;
    sf::FloatRect textureRect;
};

inline RenderableComponent::RenderableComponent(const std::string& textureFileName)
        : vertexArray(sf::TriangleStrip, 4)
{
    texture = g_TextureManager.getResource(textureFileName);
    textureRect = sf::FloatRect(0.0f, 0.0f, 
                                static_cast<float>(texture->getSize().x), 
                                static_cast<float>(texture->getSize().y));

    vertexArray[0].position = sf::Vector2f(0.0f, 0.0f);
    vertexArray[1].position = sf::Vector2f(0.0f, textureRect.height);
    vertexArray[2].position = sf::Vector2f(textureRect.width, 0.0f);
    vertexArray[3].position = sf::Vector2f(textureRect.width, textureRect.height);

    float left = textureRect.left;
    float right = textureRect.left + textureRect.width;
    float top = textureRect.top;
    float bottom = textureRect.top + textureRect.height;

    vertexArray[0].texCoords = sf::Vector2f(left, top);
    vertexArray[1].texCoords = sf::Vector2f(left, bottom);
    vertexArray[2].texCoords = sf::Vector2f(right, top);
    vertexArray[3].texCoords = sf::Vector2f(right, bottom);
}

inline RenderableComponent::RenderableComponent(const std::string& textureFileName, const sf::FloatRect& textureRect)
        : vertexArray(sf::TriangleStrip, 4)
        , textureRect(textureRect)
{
    texture = g_TextureManager.getResource(textureFileName);

    vertexArray[0].position = sf::Vector2f(0.0f, 0.0f);
    vertexArray[1].position = sf::Vector2f(0.0f, textureRect.height);
    vertexArray[2].position = sf::Vector2f(textureRect.width, 0.0f);
    vertexArray[3].position = sf::Vector2f(textureRect.width, textureRect.height);

    float left = textureRect.left;
    float right = textureRect.left + textureRect.width;
    float top = textureRect.top;
    float bottom = textureRect.top + textureRect.height;

    vertexArray[0].texCoords = sf::Vector2f(left, top);
    vertexArray[1].texCoords = sf::Vector2f(left, bottom);
    vertexArray[2].texCoords = sf::Vector2f(right, top);
    vertexArray[3].texCoords = sf::Vector2f(right, bottom);
}