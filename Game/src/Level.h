#pragma once
#include "Platform.h"

// the level (ie room) in which the game takes place at a given moment
class Level
{
public:
	explicit Level();

	float collides(ActorHitbox const* hitbox, bool isOnGround = false) const;

	void draw(sf::RenderWindow& window) const;

private:
	std::vector<Platform> platforms{};
};