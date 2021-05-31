#include "Level.h"

Level::Level()
{
	platforms.push_back(std::make_unique<Platform>(0, 800, 1000, 20));
	platforms.push_back(std::make_unique<Platform>(0, 200, 100, 10));
}

float Level::collides(ActorHitbox const* hitbox) const
{
	float maxAbs = 0;
	float res = 0;

	auto n = platforms.size();
	for (int i = 0; i < n; i++)
	{
		float dy = platforms[i]->collides(hitbox);
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
		platforms[i]->draw(window);
	}
}
