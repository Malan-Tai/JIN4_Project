#include "Platform.h"

Platform::Platform(float x, float y, float w, float h) : shape(sf::Vector2f(w, h)), rect(x, y, w, h)
{
	shape.setPosition(x, y);
	shape.setFillColor(sf::Color::Black);
}

float Platform::collides(ActorHitbox const* hitbox) const
{
	return hitbox->collides(rect);
}

void Platform::draw(sf::RenderWindow& window) const
{
	window.draw(shape);
}
