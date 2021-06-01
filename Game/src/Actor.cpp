#include "Actor.h"
#include <iostream>

Actor::Actor(AnimHolder const& holder) : handler(holder, animation::ID::MC_idle), walkAnim(animation::ID::MC_walk), idleAnim(animation::ID::MC_idle)
{
}

Actor::Actor(AnimHolder const& holder, animation::ID id, animation::ID walk) : handler(holder, id), walkAnim(walk), idleAnim(id)
{
}

animation::ID Actor::update(sf::Time const& elapsed, Level const& level)
{
	updateMoveControl();

	States state = machine.state();
	float gravity = 0;
	if (state == States::Ground || state == States::Sprint)
	{
		if (velocity.x != 0) handler.changeAnim(walkAnim);
		else handler.changeAnim(idleAnim);
	}
	else if (state == States::Fall)
	{
		gravity = 200;
	}
	else if (state == States::FastFall)
	{
		gravity = 400;
	}

	velocity += gravity * elapsed.asSeconds() * sf::Vector2f(0, 1); // gravity
	coords += elapsed.asSeconds() * velocity;

	bool isOnGround = (state == States::Ground || state == States::Sprint);
	float dy = handler.collides(level, isOnGround);
	if (!isOnGround) coords += dy * sf::Vector2f(0, 1);
	else if (dy < 0) coords += (dy + 10) * sf::Vector2f(0, 1);

	if (dy == 0 && isOnGround) machine.execute(Triggers::Fall);
	else if (dy > 0) velocity.y = 0;
	else if (dy < 0) machine.execute(Triggers::Land);

	handler.setPosition(coords);

	auto animEnd = handler.update(elapsed, velocity.x);
	if (animEnd != animation::ID::None)
	{
		auto animTrig = endAnimTriggers.find(animEnd);
		if (animTrig != endAnimTriggers.end()) machine.execute(animTrig->second);
		if (bufferedTrigger != Triggers::None) execute(bufferedTrigger);
	}
	return animEnd;
}

void Actor::hits(Actor* other)
{
	if (handler.hits(other->handler)) std::cout << "oof\n";
}

void Actor::execute(Triggers trigger)
{
	if (machine.execute(trigger) == FSM::Fsm_NoMatchingTrigger)
	{
		bufferedTrigger = trigger;
	}
	else bufferedTrigger = Triggers::None;
}

void Actor::jump()
{
	execute(Triggers::Jump);
}

void Actor::lightAttack()
{
	execute(Triggers::LightAttack);
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
