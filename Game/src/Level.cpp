#include "Level.h"

Level::Level()
{
	platforms.emplace_back(0, 800, 1000, 20);
	platforms.emplace_back(0, 700, 300, 10);
}

// returns the biggest offset (positive or negative) returned by the level's platforms
float Level::collides(ActorHitbox const* hitbox, bool isOnGround) const
{
	float maxAbs = 0;
	float res = 0;

	auto n = platforms.size();
	for (int i = 0; i < n; i++)
	{
		float dy = platforms[i].collides(hitbox, isOnGround);
		float ady = abs(dy);
		if (ady > maxAbs)
		{
			maxAbs = ady;
			res = dy;
		}
	}
	return res;
}

void Level::draw(sf::RenderWindow& window) const
{
	auto n = platforms.size();
	for (int i = 0; i < n; i++)
	{
		platforms[i].draw(window);
	}
}
