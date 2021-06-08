#include "Platform.h"

Platform::Platform(float x, float y, float w, float h) : shape(sf::Vector2f(w, h)), rect(x, y, w, h)
{
	shape.setPosition(x, y);
	shape.setFillColor(sf::Color::Black);
}

// returns the vertical offset the hitbox needs so as not to collide with the platform anymore
// negative is the hitbox is above, positive otherwise
// if the actor is on the ground, moves the hitbox up a bit, to be more lenient and fluid
float Platform::collides(ActorHitbox const* hitbox, bool isOnGround) const
{
	sf::FloatRect sentRect{ rect };
	if (isOnGround)
	{
		sentRect.top -= 10;
		sentRect.height += 10;
	}
	return hitbox->collides(sentRect);
}

void Platform::draw(sf::RenderWindow& window) const
{
	window.draw(shape);
}
