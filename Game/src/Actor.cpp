#include "Actor.h"
#include <iostream>

Actor::Actor(AnimHolder const& holder) : handler(holder, animation::ID::MC_idle)
{
}

Actor::Actor(AnimHolder const& holder, animation::ID id) : handler(holder, id)
{
}

animation::ID Actor::update(sf::Time const& elapsed)
{
	States state = machine.state();
	updateMoveControl();

	float gravity = 0;
	if (state == States::Fall)
	{
		gravity = 200;
	}
	else if (state == States::FastFall)
	{
		gravity = 400;
	}
	velocity += gravity * elapsed.asSeconds() * sf::Vector2f(0, 1); // gravity

	coords += elapsed.asSeconds() * velocity;

	handler.setPosition(coords);

	auto animEnd = handler.update(elapsed, velocity.x);
	if (animEnd != animation::ID::None)
	{
		auto animTrig = endAnimTriggers.find(animEnd);
		if (animTrig != endAnimTriggers.end()) machine.execute(animTrig->second);
	}
	return animEnd;
}

void Actor::hits(Actor* other)
{
	handler.hits(other->handler);
}

void Actor::jump()
{
	machine.execute(Triggers::Jump);
}

void Actor::lightAttack()
{
	machine.execute(Triggers::LightAttack);
}

void Actor::setVelocity(sf::Vector2f unitVelocity)
{
	velocity = speed * unitVelocity;
}

void Actor::setHorizontalVelocity(float dx)
{
	float dy = velocity.y;
	velocity = sf::Vector2f(dx * speed * moveControl, dy);
	if (machine.state() == States::Roll)
	{ 
		int xDir = handler.getXDir();
		xDir = xDir / abs(xDir);
		velocity = sf::Vector2f(xDir * speed * 3, 0);
	}
}

void Actor::updateMoveControl()
{
	auto state = machine.state();
	switch (state)
	{
	case States::Fall:
		moveControl = 0.5f;
		return;
	case States::FastFall:
		moveControl = 0;
		return;
	case States::FallAttack:
		moveControl = 0;
		return;
	case States::Sprint:
		moveControl = 2;
		return;
	case States::LightAttack:
		moveControl = 0;
		return;
	case States::HeavyAttack:
		moveControl = 0;
		return;
	default:
		moveControl = 1;
		return;
	}
}

void Actor::draw(sf::RenderWindow& window) const
{
	handler.draw(window);
	//window.draw(sprite);
}

void Actor::changeAnim(animation::ID id)
{
	handler.changeAnim(id);
}

Prototype* Actor::clone() const
{
	return new Actor(handler.getHolder());
}
