#include "Actor.h"
#include <iostream>
#include "Lens.h"
#include "ActorPipe.h"

Actor::Actor(ActorPipe* pipe, AnimHolder const& holder) : handler(holder, animation::ID::MC_idle), idleAnim(animation::ID::MC_idle), walkAnim(animation::ID::MC_walk),
														  AI(ArtificialIntelligence{ hitboxes::Layers::Enemy }), pipe(pipe)
{
}

Actor::Actor(ActorPipe* pipe, AnimHolder const& holder, animation::ID id, animation::ID walk) : handler(holder, id), idleAnim(id), walkAnim(walk),
																								AI(ArtificialIntelligence{ hitboxes::Layers::Enemy }), pipe(pipe)
{
}

Actor::Actor(ActorPipe* pipe, AnimHolder const& holder, animation::ID id, animation::ID walk, float speed) : handler(holder, id), idleAnim(id), walkAnim(walk), speed(speed),
																											 AI(ArtificialIntelligence{ hitboxes::Layers::Enemy }), pipe(pipe)
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
	else if (state == States::FastFall || state == States::Thrown)
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

	if (state == States::Staggered)
	{
		staggerTime += elapsed;
		if (staggerTime.asMilliseconds() > staggerMaxTime)
		{
			staggerTime = sf::Time::Zero;
			machine.execute(Triggers::Recover);
		}
	}

	if (state == States::Grabbing && grabbed != nullptr)
	{
		grabTime += elapsed;
		if (grabTime.asMilliseconds() > grabMaxTime)
		{
			grabTime = sf::Time::Zero;
			machine.execute(Triggers::EndGrab);
			grabbed->machine.execute(Triggers::EndGrab);
			grabbed = nullptr;
		}
	}

	spendStamina(handler.getContinuousStaminaCost() * elapsed.asSeconds());
	if (noStamRegen.asMilliseconds() > 0)
	{
		noStamRegen -= elapsed;
	}
	if (noStamRegen.asMilliseconds() <= 0 && stamina < maxStamina)
	{
		stamina = std::min(stamina + staminaRegen * elapsed.asSeconds(), (float)maxStamina);
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

void Actor::hits(Actor* other, LensColors leftLens, LensColors rightLens)
{
	if (other->seen(leftLens, rightLens) && handler.hits(other->handler))
	{
		if (machine.state() == States::TryGrabbing)
		{
			if (other->machine.state() != States::Staggered) return;

			grabbed = other;
			other->machine.execute(Triggers::Grab);
			machine.execute(Triggers::Grab);
			return;
		}

		machine.execute(Triggers::DoHit);
		other->getHit(strength, handler.getPoiseDamage());
	}
}

bool Actor::seen(LensColors leftLens, LensColors rightLens) const
{
	return (lensColor == LensColors::None && leftLens == lensColor && rightLens == lensColor)
		|| (lensColor != LensColors::None && (leftLens == lensColor ||rightLens == lensColor));
}

#if TESTS
M const& Actor::getMachine() const
{
	return machine;
}

sf::Vector2f Actor::getVelocity() const
{
	return velocity;
}

Triggers Actor::getBuffer() const
{
	return bufferedTrigger;
}
#endif

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

void Actor::spendStamina(float cost)
{
	if (cost <= 0) return;

	stamina = std::max(0.f, stamina - cost);
	noStamRegen = sf::milliseconds(noStamRegenTime);

	std::cout << "spent stamina, left = " << stamina << std::endl;
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

void Actor::getHit(int dmg, int poiseDmg)
{
	hp -= dmg;
	std::cout << "oofed : " << dmg << "\n";

	if (handler.takePoiseDamage(poiseDmg)) stabilityHP -= cancelledStabilityDamage;

	if (stabilityHP <= 0)
	{
		stabilityHP = stabilityMaxHP;
		machine.execute(Triggers::Stagger);
	}
	else machine.execute(Triggers::GetHit);
}

void Actor::getThrown(int dx, int dy)
{
	float vx = 0.f;
	float vy = 0.f;
	vx = dx * 700;
	if (dy > 0) vy = -800.f;
	if (dy == 0) vy = -400.f; // left or rigth throw
	velocity = sf::Vector2f{ vx, vy };

	machine.execute(Triggers::Throw);
}

void Actor::setHorizontalVelocity(float dx)
{
	auto state = machine.state();
	if (state == States::Thrown) return;

	float dy = velocity.y;
	velocity = sf::Vector2f(dx * speed * moveControl, dy);
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
	case States::Grabbing:
		moveControl = 0;
		return;
	case States::Grabbed:
		moveControl = 0;
		return;
	case States::Thrown:
		moveControl = 0;
		return;
	default:
		moveControl = 1;
		return;
	}
}

void Actor::draw(sf::RenderWindow& window, LensColors leftLens, LensColors rightLens) const
{
	if (seen(leftLens, rightLens)) handler.draw(window, hp, maxHP);
}

void Actor::changeAnim(animation::ID id)
{
	spendStamina(handler.changeAnim(id));
}

bool Actor::toRemove() const
{
	return machine.state() == States::ToBeRemoved;
}

void Actor::grab()
{
	execute(Triggers::Grab);
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

void Actor::doThrow(int dx, int dy)
{
	if (dx == 0 && dy == 0) return;
	if (machine.state() != States::Grabbing || grabbed == nullptr) return;

	grabbed->getThrown(dx, dy);
	grabbed = nullptr;
	machine.execute(Triggers::Throw);
}

void Actor::chooseTarget(std::vector<Actor const*>& actors, LensColors leftLens, LensColors rightLens)
{
	AI.chooseTarget(actors, coords, leftLens, rightLens);
}

void Actor::takeDecision()
{
	AI_decision decision = AI.makeDecision(coords);
	setHorizontalVelocity(decision.xDir);
	execute(decision.trigger);
}

std::unique_ptr<Actor> Actor::clone() const
{
	return std::make_unique<Actor>(pipe, handler.getHolder(), idleAnim, walkAnim, speed);
}
