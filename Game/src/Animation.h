#pragma once
#include <SFML/Graphics.hpp>
#include <pugixml.hpp>
#include "ResourceHolder.h"
#include "ActorHitbox.h"

class Animation
{
public:
	explicit Animation() = default;

	bool loadFromFile(std::string const& basename);

	void setSprite(sf::Sprite& sprite, int frame, bool inv = false) const;
	std::vector<Hitbox const*> getHitboxes(int frame) const;

	//void addHitboxes(int frame, std::vector<Hitbox>& _hitboxes, std::vector<Hitbox>& _hurtboxes) const;

	int nbFrames;
	float timePerFrame = 500;
	bool looping = true;
	bool reverseLoop = false; // loop from 0 to n then n to 0 etc

private:
	sf::Texture spritesheet;
	std::vector<std::vector<Hitbox const*>> hitboxes;

	int frameWidth;
	int height;
};