#pragma once
#include "SFML/Graphics.hpp"
#include "ActorHitbox.h"

class Platform
{
public:
	explicit Platform(float x, float y, float w, float h);

	float collides(ActorHitbox const* hitbox) const;

	void draw(sf::RenderWindow& window) const;

private:
	sf::RectangleShape shape;
	sf::FloatRect rect;
};