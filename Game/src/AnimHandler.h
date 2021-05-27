#pragma once
#include "Animation.h"

class AnimHandler
{
public:
	void update(sf::Time const& elapsed);

	void draw(sf::RenderWindow& window) const;

private:
	int frame = 0;
	int nbFrames = 0;
	sf::Sprite spritesheet;
};