#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"

namespace texture
{
	enum class ID
	{
		background,
	};
}

namespace animation
{
	enum class ID
	{
		None,
		MC_idle,
		MC_roll,
		MC_jump,
		MC_fall,
		MC_light_attack,
		MC_heavy_attack,
		MC_grab,
		MC_walk,
		MC_roll_attack,
		MC_hurt,
		monster_idle,
		monster_walk,
		monster_hurt,
		fireball
	};
}

template <typename Identifier, typename Resource>
class ResourceHolder
{
public:
	void load(Identifier id, std::string const& filename);

	Resource const& get(Identifier id) const;

private:
	std::map<Identifier, std::unique_ptr<Resource>> resourceMap{};
};

using TextureHolder = ResourceHolder<texture::ID, sf::Texture>;
using AnimHolder = ResourceHolder<animation::ID, class Animation>;

template <typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, std::string const& filename)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	resourceMap.emplace(id, std::move(resource));
}

template <typename Identifier, typename Resource>
Resource const& ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
	auto found = resourceMap.find(id);
	if (found == resourceMap.end())
		throw std::runtime_error("ResourceHolder::get - Failed");

	return *found->second.get();
}

