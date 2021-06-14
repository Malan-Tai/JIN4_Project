#pragma once
#include "Hitbox.h"

// an instanciated hitbox, which exists in the game world
class ActorHitbox
{
public:
	explicit ActorHitbox(Hitbox const* hitbox, sf::Vector2f pos, bool inv, float width);

	bool intersect(ActorHitbox const* other) const;
	float collides(sf::FloatRect const& _rect) const;

	void setPosition(sf::Vector2f pos, bool inv);

	hitboxes::Type getType() const;
	hitboxes::Layers getLayer() const;

private:
	Hitbox const* original;
	float frameWidth;

	sf::FloatRect rect;
};