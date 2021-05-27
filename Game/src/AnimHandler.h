#pragma once
#include <pugixml.hpp>
#include "ResourceHolder.h"

class AnimHandler
{
public:
	explicit AnimHandler(std::string& resource, texture::ID identifier);

	void update(sf::Time const& elapsed);

	void draw(sf::RenderWindow& window) const;

private:
	int frame = 0;
	int nbFrames = 0;
	sf::RectangleShape rectangle;
	sf::Sprite spritesheet;
	pugi::xml_document hitboxes;

	TextureHolder& textureHolder;
};