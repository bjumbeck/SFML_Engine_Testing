#include <cassert>

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::loadResource(Identifier id, const std::string& filename)
{
    std::unique_ptr<Resource> resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceManager::LoadResource - Failed to load " + filename);

    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceManager<Resource, Identifier>::loadResource(Identifier id, const std::string& filename,
                                                         const Parameter& secondParam)
{
    std::unique_ptr<Resource> resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResourceManager::LoadResource - Failed to load " + filename);

    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource* ResourceManager<Resource, Identifier>::getResource(Identifier id)
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());

    return found->second.get();
}

template <typename  Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::insertResource(Identifier identifier, std::unique_ptr<Resource> resource)
{
    auto inserted = resourceMap.insert(std::make_pair(identifier, std::move(resource)));
    assert(inserted.second);
}
