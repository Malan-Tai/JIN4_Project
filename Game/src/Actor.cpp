#include "Actor.h"
#include <iostream>
#include "ActorPipe.h"

Actor::Actor(AnimHolder const& holder) : handler(holder, animation::ID::MC_idle), idleAnim(animation::ID::MC_idle), walkAnim(animation::ID::MC_walk), AI(ArtificialIntelligence{ hitboxes::Layers::Enemy })
{
}

Actor::Actor(AnimHolder const& holder, animation::ID id, animation::ID walk) : handler(holder, id), idleAnim(id), walkAnim(walk), AI(ArtificialIntelligence{ hitboxes::Layers::Enemy })
{
}

Actor::Actor(AnimHolder const& holder, animation::ID id, animation::ID walk, float speed) : handler(holder, id), idleAnim(id), walkAnim(walk), speed(speed), AI(ArtificialIntelligence{ hitboxes::Layers::Enemy })
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
		gravity = 500;
	}
	else if (state == States::FastFall)
	{
		gravity = 1000;
	}

	takeDecision();

	velocity += gravity * elapsed.asSeconds() * sf::Vector2f(0, 1); // gravity
	coords += elapsed.asSeconds() * velocity;

	handler.setPosition(coords);

	bool isOnGround = (state == States::Ground || state == States::Sprint);
	float dy = handler.collides(level, isOnGround);
	if (!isOnGround) coords += dy * sf::Vector2f(0, 1);
	else if (dy < 0) coords += (dy + 10) * sf::Vector2f(0, 1);

	if (dy == 0 && isOnGround) machine.execute(Triggers::Fall);
	else if (dy > 0) velocity.y = 0;
	else if (dy < 0) machine.execute(Triggers::Land);

	forgetPrevState += elapsed;
	if (state != States::Ground || (state == States::Ground && forgetPrevState.asMilliseconds() > forgetPrevStateTime))
	{
		previousState = state;
		forgetPrevState = sf::Time::Zero;
	}

	forgetCombo += elapsed;
	if (forgetCombo.asMilliseconds() > forgetComboTime)
	{
		comboDamage = 0;
	}

	if (state == States::Staggered)
	{
		staggerTime += elapsed;
		if (staggerTime.asMilliseconds() > staggerMaxTime)
		{
			staggerTime = sf::Time::Zero;
			machine.execute(Triggers::Recover);
			comboDamage = 0;
		}
	}

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
	if (handler.hits(other->handler))
	{
		machine.execute(Triggers::DoHit);
		other->getHit(strength);
	}
}

void Actor::execute(Triggers trigger)
{
	if (machine.execute(trigger) == FSM::Fsm_NoMatchingTrigger)
	{
		bufferedTrigger = trigger;
	}
	else bufferedTrigger = Triggers::None;
}

hitboxes::Layers Actor::getLayer() const
{
	return AI.getLayer();
}

void Actor::jump()
{
	execute(Triggers::Jump);
}

void Actor::lightAttack()
{
	execute(Triggers::LightAttack);
}

void Actor::heavyAttack()
{
	execute(Triggers::HeavyAttack);
}

void Actor::setCoords(sf::Vector2f c)
{
	coords = c;
}

void Actor::setVelocity(sf::Vector2f unitVelocity)
{
	velocity = speed * unitVelocity;
}

void Actor::getHit(int dmg)
{
	forgetCombo = sf::Time::Zero;
	comboDamage += dmg;
	hp -= dmg;
	std::cout << "oofed : " << dmg << "\n";

	if (comboDamage >= maxHP / 3) machine.execute(Triggers::Stagger);
	else machine.execute(Triggers::GetHit);
}

void Actor::setHorizontalVelocity(float dx)
{
	float dy = velocity.y;
	velocity = sf::Vector2f(dx * speed * moveControl, dy);
	auto state = machine.state();
	if (state == States::Roll)
	{
		float xDir = handler.getXDir();
		xDir = xDir / abs(xDir);
		velocity = sf::Vector2f(xDir * speed * 1.5f, 0);
	}
	else if (state == States::RollLightAttack)
	{
		float xDir = handler.getXDir();
		xDir = xDir / abs(xDir);
		velocity = sf::Vector2f(xDir * speed * 0.75f, 0);
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
	case States::GotHit:
		moveControl = 0;
		return;
	case States::Staggered:
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

bool Actor::toRemove() const
{
	return machine.state() == States::ToBeRemoved;
}

float Actor::distanceTo(sf::Vector2f point) const
{
	float dx = point.x - coords.x;
	float dy = point.y - coords.y;
	return sqrt(dx * dx + dy * dy);
}

sf::Vector2f const& Actor::getCoords() const
{
	return coords;
}

void Actor::chooseTarget(std::vector<Actor const*>& actors)
{
	AI.chooseTarget(actors, coords);
}

void Actor::takeDecision()
{
	AI_decision decision = AI.makeDecision(coords);
	setHorizontalVelocity(decision.xDir);
	execute(decision.trigger);
}

std::unique_ptr<Actor> Actor::clone() const
{
	return std::make_unique<Actor>(handler.getHolder(), idleAnim, walkAnim, speed);
}
