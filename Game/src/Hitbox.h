#pragma once
#include "SFML/Graphics.hpp"

namespace hitboxes
{
	enum class Type
	{
		None,
		Hit,
		Hurt
	};
	enum class Layers
	{
		None,
		Ally,
		Enemy,
		All
	};

	Type strToType(std::string const& str);
	Layers strToLayer(std::string const& str);
}

class Hitbox
{
public:
	explicit Hitbox(sf::FloatRect rect, hitboxes::Layers layer, hitboxes::Type type);

	hitboxes::Type getType() const;
	sf::FloatRect getRect() const;
	hitboxes::Layers getLayer() const;

protected:
	sf::FloatRect rect;
	hitboxes::Layers layer;
	hitboxes::Type type;
};