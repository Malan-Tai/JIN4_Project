#include "Actor.h"

Actor::Actor(TextureHolder const& holder) : holder(holder)
{
	sprite.setTexture(holder.get(texture::ID::mainCharacter_idle), true);
}

void Actor::update(sf::Time const& elapsed)
{
	coords += elapsed.asSeconds() * speed;

	sprite.setPosition(coords);
}

void Actor::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}

Prototype* Actor::clone() const
{
	return new Actor(holder);
}
