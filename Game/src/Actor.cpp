#include "Actor.h"
#include <iostream>

Actor::Actor(TextureHolder const& holder) : holder(holder)
{
	sprite.setTexture(holder.get(texture::ID::mainCharacter_idle), true);

	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		{ States::Ground, States::Fall, Triggers::Jump, nullptr, nullptr },
		{ States::Ground, States::Roll, Triggers::PressRoll, nullptr, [] { std::cout << "roll\n"; }},
		{ States::Roll, States::Ground, Triggers::EndRoll, nullptr, [] { std::cout << "end roll\n"; }},
		{ States::Ground, States::Sprint, Triggers::HoldSprint, nullptr, nullptr },
		{ States::Sprint, States::Ground, Triggers::ReleaseSprint, nullptr, nullptr },
	};

	machine.add_transitions(transitions);
}

void Actor::update(sf::Time const& elapsed)
{
	States state = machine.state();
	moveControl = 1;

	float gravity = 0;
	if (state == States::Fall)
	{
		gravity = 200;
		moveControl = 0.5f;
	}
	else if (state == States::FastFall)
	{
		gravity = 400;
		moveControl = 0;
	}
	velocity += gravity * elapsed.asSeconds() * sf::Vector2f(0, 1); // gravity

	coords += elapsed.asSeconds() * velocity;

	sprite.setPosition(coords);
}

void Actor::jump()
{
	float vx = velocity.x / 2;
	velocity = sf::Vector2f{ vx, -500 };

	machine.execute(Triggers::Jump);
}

void Actor::setVelocity(sf::Vector2f unitVelocity)
{
	velocity = speed * unitVelocity;
}

void Actor::setHorizontalVelocity(float dx)
{
	float speedBoost = 1;
	if (machine.state() == States::Sprint) speedBoost = 2;

	float dy = velocity.y;
	velocity = sf::Vector2f(dx * speed * moveControl * speedBoost, dy);
}

void Actor::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}

Prototype* Actor::clone() const
{
	return new Actor(holder);
}
