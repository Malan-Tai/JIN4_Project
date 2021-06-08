#pragma once
#include <SFML/Graphics.hpp>
#include <pugixml.hpp>
#include "ResourceHolder.h"
#include "ActorHitbox.h"

#define TESTS 1

class Animation
{
public:
	explicit Animation() = default;

	bool loadFromFile(std::string const& basename);

	void setSprite(sf::Sprite& sprite, int frame, bool inv = false) const;
	std::vector<Hitbox const*> getHitboxes(int frame) const;

	int nbFrames;
	float timePerFrame = 500;
	bool looping = true;
	bool reverseLoop = false; // loop from 0 to n then n to 0 etc

#if TESTS
	int hitboxesLength() const;
#endif

private:
	sf::Texture spritesheet;
	std::vector<std::vector<Hitbox const*>> hitboxes;

	int frameWidth;
	int height;
};