#pragma once
#include "Prototype.h"
#include "AnimHandler.h"

class Actor : public Prototype
{
public:
	explicit Actor(TextureHolder const& holder);

	void update(sf::Time const& elapsed);

	void draw(sf::RenderWindow& window) const;

	Prototype* clone() const override;

private:
	sf::Vector2f speed{ 100, 0 };
	sf::Vector2f coords{ 50, 50 };

	sf::Sprite sprite;

	TextureHolder const& holder;
};