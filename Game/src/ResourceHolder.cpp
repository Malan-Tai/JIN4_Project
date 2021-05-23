#include "ResourceHolder.h"

template <typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, std::string const& filename)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	resourceMap.insert(id, std::move(resource));
}

template <typename Identifier, typename Resource>
Resource const& ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
	auto found = resourceMap.find(id);
	if (found == resourceMap.end())
		throw std::runtime_error("ResourceHolder::get - Failed to get " + id);

	return *found->second.get();
}