#pragma once
#include "Hitbox.h"

class ActorHitbox : public Hitbox
{
public:
	explicit ActorHitbox(Hitbox const* hitbox, sf::Vector2f pos, bool inv, float width);

	bool intersect(ActorHitbox const& other) const;

	void setPosition(sf::Vector2f pos, bool inv);

private:
	Hitbox const* original;
	float frameWidth;
};