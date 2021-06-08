#pragma once
#include "SFML/Graphics.hpp"

namespace hitboxes
{
	// types of hitboxes :
	// Hit is an actor hitbox, where it can be hit
	// Hurt is an attack hurtbox, where it can hit actors
	enum class Type
	{
		None,
		Hit,
		Hurt
	};
	// layers of hitboxes :
	// a hurtbox needs to be from a different layer than a hitbox in order to hit it
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

// a virtual hitbox, stocked by an animation
// it is then given to an actor instance to create an instanciated ActorHitbox
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