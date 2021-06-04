#pragma once
#include "Platform.h"

class Level
{
public:
	explicit Level();

	float collides(ActorHitbox const* hitbox, bool isOnGround = false) const;

	void draw(sf::RenderWindow& window) const;

private:
	std::vector<Platform> platforms{};
};