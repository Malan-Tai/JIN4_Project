#pragma once
#include "Prototype.h"
#include "AnimHandler.h"

class Actor : public Prototype
{
public:
	explicit Actor(TextureHolder const& holder);
	Prototype* clone() const override;

	void update(sf::Time const& elapsed);

	void jump();
	void addVelocity(sf::Vector2f unitVelocity);

	void draw(sf::RenderWindow& window) const;

private:
	float speed = 50;

	sf::Vector2f velocity{ 0, 0 };
	sf::Vector2f coords{ 500, 500 };

	sf::Sprite sprite;

	TextureHolder const& holder;
};