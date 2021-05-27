#pragma once
#include <SFML/Graphics.hpp>
#include <pugixml.hpp>
#include "ResourceHolder.h"

class Animation
{
public:
	explicit Animation() = default;

	bool loadFromFile(std::string const& basename);

	void setSprite(sf::Sprite& sprite, int frame, bool inv = false) const;

	int nbFrames;
	float timePerFrame;

private:
	sf::Texture spritesheet;
	std::vector<std::vector<sf::FloatRect>> hitboxes;
	std::vector<std::vector<sf::FloatRect>> hurtboxes;

	int frameWidth;
	int height;
};