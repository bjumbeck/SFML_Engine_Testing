#pragma once

// TODO: Ended up just using the resource manager from the SFML book as a place
// holder until I can get the time to create a better solution for my particular case.

#include <string>
#include <memory>
#include <map>
#include <SFML/Graphics/Texture.hpp>

template <typename Resource, typename Identifier>
class ResourceManager
{
    public:
        void LoadResource(Identifier id, const std::string& filename);

        template <typename Parameter>
        void LoadResource(Identifier id, const std::string& filename, const Parameter& secondParam);

        std::shared_ptr<Resource> GetResource(Identifier id);

    private:
        void InsertResource(Identifier identifier, std::shared_ptr<Resource> resource);

    private:
        std::map<Identifier, std::shared_ptr<Resource>> resourceMap;
};

#include "ResourceManager.inl"

// TODO: Delete this crap ASAP
static ResourceManager<sf::Texture, std::string> g_TextureManager;