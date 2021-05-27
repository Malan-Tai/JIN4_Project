#include "Actor.h"

Actor::Actor(TextureHolder const& holder) : holder(holder)
{
	sprite.setTexture(holder.get(texture::ID::mainCharacter_idle), true);
}

void Actor::update(sf::Time const& elapsed)
{
	velocity += 200 * elapsed.asSeconds() * sf::Vector2f(0, 1); // gravity
	coords += elapsed.asSeconds() * velocity;

	sprite.setPosition(coords);
}

void Actor::jump()
{
	float vx = velocity.x / 2;
	velocity = sf::Vector2f{ vx, -500 };
}

void Actor::setVelocity(sf::Vector2f unitVelocity)
{
	velocity = speed * unitVelocity;
}

void Actor::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}

Prototype* Actor::clone() const
{
	return new Actor(holder);
}
