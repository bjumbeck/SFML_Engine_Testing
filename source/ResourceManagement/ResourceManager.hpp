#pragma once

// TODO: Ended up just using the resource manager from the SFML book as a place
// holder until I can get the time to create a better solution for my particular use case.

#include <string>
#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

template <typename Resource, typename Identifier>
class ResourceManager
{
    public:
        void loadResource(const Identifier& id, const std::string& filename);

        template <typename Parameter>
        void loadResource(const Identifier& id, const std::string& filename, const Parameter& secondParam);

        void unloadResources();

        Resource* getResource(const Identifier& id);

    private:
        void insertResource(const Identifier& id, std::unique_ptr<Resource> resource);

    private:
        std::unordered_map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

#include "ResourceManager.inl"

// TODO: Delete this crap ASAP
static ResourceManager<sf::Texture, std::string> g_TextureManager;