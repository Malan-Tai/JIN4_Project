#pragma once
#include "Prototype.h"
#include "AnimHandler.h"

class Actor : public Prototype
{
public:
	explicit Actor();

	void update(sf::Time const& elapsed);

	void draw(sf::RenderWindow* window) const;

private:
	AnimHandler anim;
};