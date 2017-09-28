#include <cassert>

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::LoadResource(Identifier id, const std::string& filename)
{
    std::shared_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceManager::LoadResource - Failed to load " + filename);

    InsertResource(id, resource);
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceManager<Resource, Identifier>::LoadResource(Identifier id, const std::string& filename,
                                                         const Parameter& secondParam)
{
    std::shared_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResourceManager::LoadResource - Failed to load " + filename);

    InsertResource(id, resource);
}

template <typename Resource, typename Identifier>
std::shared_ptr<Resource> ResourceManager<Resource, Identifier>::GetResource(Identifier id)
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());

    return found->second;
}

template <typename  Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::InsertResource(Identifier identifier, std::shared_ptr<Resource> resource)
{
    auto inserted = resourceMap.insert(std::make_pair(identifier, resource));
    assert(inserted.second);
}
