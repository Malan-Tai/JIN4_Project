#pragma once
#include <SFML/Graphics.hpp>
#include <pugixml.hpp>
#include "ResourceHolder.h"
#include "ActorHitbox.h"

#define TESTS 0

// a data class : gets loaded from a spritesheet and an xml data file
class Animation
{
public:
	explicit Animation() = default;

	bool loadFromFile(std::string const& basename);

	void setSprite(sf::Sprite& sprite, int frame, bool inv = false) const;
	std::vector<Hitbox const*> getHitboxes(int frame) const;
	int getTimeForFrame(int frame) const;
	float getDamageMultiplier(int frame) const;

	int nbFrames;
	bool looping = true;
	bool reverseLoop = false; // loop from 0 to n then n to 0 etc

	bool takesPoiseDmg = false;
	float poiseHP = 0;
	float poiseDamage = 0;

	bool continuousStamCost = false;
	float staminaCost = 0;

#if TESTS
	int hitboxesLength() const;
#endif

private:
	sf::Texture spritesheet;
	std::vector<std::vector<Hitbox const*>> hitboxes;

	int frameWidth;
	int height;

	std::vector<int> timePerFrames;
	std::vector<float> damageMultipliers;
};