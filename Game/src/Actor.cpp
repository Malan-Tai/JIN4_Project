#include "Actor.h"
#include <iostream>

Actor::Actor(AnimHolder const& holder) : handler(holder, animation::ID::MC_idle)
{
	//sprite.setTexture(holder.get(texture::ID::MC_idle), true);

	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		{ States::Ground, States::Fall, Triggers::Jump, nullptr, [this] { this->doJump(); }},
		{ States::Ground, States::Roll, Triggers::PressRoll, nullptr, [] { std::cout << "roll\n"; }},
		{ States::Fall, States::Roll, Triggers::PressRoll, nullptr, [this] { std::cout << "air roll\n"; this->setVelocity(sf::Vector2f{0, 0}); }},
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

	handler.setPosition(coords);
	handler.update(elapsed, velocity.x);
}

void Actor::doJump()
{
	float vx = velocity.x / 2;
	velocity = sf::Vector2f{ vx, -500 };
}

void Actor::jump()
{
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
	if (machine.state() == States::Roll)
	{ 
		int xDir = handler.getXDir();
		xDir = xDir / abs(xDir);
		velocity = sf::Vector2f(xDir * speed * 3, 0);
	}
}

void Actor::draw(sf::RenderWindow& window) const
{
	handler.draw(window);
	//window.draw(sprite);
}

Prototype* Actor::clone() const
{
	return new Actor(handler.getHolder());
}
