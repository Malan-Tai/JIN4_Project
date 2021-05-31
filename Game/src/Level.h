#pragma once
#include "Platform.h"

class Level
{
public:
	explicit Level();

	float collides(ActorHitbox const* hitbox) const;

	void draw(sf::RenderWindow& window) const;

private:
	std::vector<std::unique_ptr<Platform>> platforms{};
};