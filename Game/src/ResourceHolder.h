#pragma once
#include <SFML/Graphics.hpp>

namespace texture
{
	enum class ID
	{
		background,
		mainCharacter_idle,
	};
}

template <typename Identifier, typename Resource>
class ResourceHolder
{
public:
	void load(Identifier id, std::string const& filename);

	Resource const& get(Identifier id) const;

private:
	std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

typedef ResourceHolder<texture::ID, sf::Texture> TextureHolder;
