#include "ControllableActor.h"
#include "ActorPipe.h"
#include <iostream>

ControllableActor::ControllableActor(ActorPipe* pipe, AnimHolder const& holder, bool init_lens) : Actor(pipe, holder), prev(this), next(this)
{
	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		// air
		{ States::Ground, States::Fall, Triggers::Jump, nullptr, [this] { velocity = sf::Vector2f{ 0, -300 }; handler.changeAnim(animation::ID::MC_jump); jumps++;  coords += sf::Vector2f{0, -20}; airRolls = 0; }},
		{ States::Ground, States::Fall, Triggers::Fall, nullptr, [this] { handler.changeAnim(animation::ID::MC_fall); jumps++; }},
		{ States::Fall, States::Fall, Triggers::Jump, [this] {return jumps < 2; }, [this] { velocity = sf::Vector2f{ 0, -300 }; handler.changeAnim(animation::ID::MC_jump); jumps++; airRolls = 0; }},
		{ States::Fall, States::Ground, Triggers::Land, nullptr, [this] { velocity = sf::Vector2f{ 0, 0 }; handler.changeAnim(animation::ID::MC_idle); jumps = 0; airRolls = 0; }},
		{ States::Fall, States::FastFall, Triggers::PressDown, nullptr, nullptr },
		{ States::FastFall, States::Fall, Triggers::ReleaseDown, nullptr, nullptr },
		{ States::FastFall, States::Ground, Triggers::Land, nullptr, [this] { velocity = sf::Vector2f{ 0, 0 }; handler.changeAnim(animation::ID::MC_idle); jumps = 0; }},

		// roll
		{ States::Ground, States::Roll, Triggers::PressRoll, nullptr, [this] { changeAnim(animation::ID::MC_roll); }},
		{ States::Fall, States::Roll, Triggers::PressRoll, [this] { return airRolls < 1; }, [this] { changeAnim(animation::ID::MC_roll); airRolls++; }},
		{ States::Roll, States::Ground, Triggers::EndRoll, nullptr, [this] { changeAnim(animation::ID::MC_idle); }},

		// sprint
		{ States::Ground, States::Sprint, Triggers::HoldSprint, nullptr, nullptr },
		{ States::Sprint, States::Ground, Triggers::ReleaseSprint, nullptr, nullptr },
		{ States::Sprint, States::Fall, Triggers::Fall, nullptr, [this] { handler.changeAnim(animation::ID::MC_fall); jumps++; }},

		// attacks
		{ States::Ground, States::LightAttack, Triggers::LightAttack, [this] { return previousState != States::Roll; }, [this] { changeAnim(getAttackAnim(false)); if (!meleeWeapon) shoot(false); }},
		{ States::Ground, States::RollLightAttack, Triggers::LightAttack, [this] { return previousState == States::Roll; }, [this] { changeAnim(animation::ID::MC_roll_attack); }},
		{ States::LightAttack, States::Ground, Triggers::EndLightAttack, nullptr, [this] { changeAnim(animation::ID::MC_idle); }},
		{ States::RollLightAttack, States::Ground, Triggers::EndLightAttack, nullptr, [this] { changeAnim(animation::ID::MC_idle); }},

		{ States::Ground, States::HeavyAttack, Triggers::HeavyAttack, nullptr, [this] { changeAnim(getAttackAnim(true)); if (!meleeWeapon) shoot(true); }},
		{ States::HeavyAttack, States::Ground, Triggers::EndHeavyAttack, nullptr, [this] { changeAnim(animation::ID::MC_idle); }},

		// weapons (useful because of buffer)
		{ States::Ground, States::Ground, Triggers::SwitchWeaponRange, [this] { return previousState != States::LightAttack; }, [this] { meleeWeapon = !meleeWeapon; }},
		{ States::Ground, States::LightAttack, Triggers::SwitchWeaponRange, [this] { return previousState == States::LightAttack; }, [this] { meleeWeapon = !meleeWeapon; changeAnim(getAttackAnim(false)); if (!meleeWeapon) shoot(false); }},
		{ States::Ground, States::Ground, Triggers::SwitchWeaponSize, nullptr, [this] { bigWeapon = !bigWeapon; }},
		{ States::Fall, States::Fall, Triggers::SwitchWeaponRange, nullptr, [this] { meleeWeapon = !meleeWeapon; }},
		{ States::Fall, States::Fall, Triggers::SwitchWeaponSize, nullptr, [this] { bigWeapon = !bigWeapon; }},
		{ States::Sprint, States::Sprint, Triggers::SwitchWeaponRange, nullptr, [this] { meleeWeapon = !meleeWeapon; }},
		{ States::Sprint, States::Sprint, Triggers::SwitchWeaponSize, nullptr, [this] { bigWeapon = !bigWeapon; }},

		// hits
		{ States::Ground, States::GotHit, Triggers::GetHit, nullptr, [this] { velocity = sf::Vector2f{ 0, 0 }; handler.changeAnim(animation::ID::MC_hurt); }},
		{ States::GotHit, States::Ground, Triggers::Recover, [this] { return hp > 0; }, [this] { handler.changeAnim(idleAnim); }},
		{ States::GotHit, States::ToBeRemoved, Triggers::Recover, [this] { return hp <= 0 && next != this; }, [this] { handler.changeAnim(idleAnim); this->pipe->switchControlled(this); std::cout << "dead, switch clone\n"; }},
		{ States::GotHit, States::Ground, Triggers::Recover, [this] { return hp <= 0 && next == this; }, [this] { handler.changeAnim(idleAnim); std::cout << "dead, game over, not implemented yet\n"; }},

		// grab
		{ States::Ground, States::TryGrabbing, Triggers::Grab, nullptr, [this] { handler.changeAnim(animation::ID::MC_grab); }},
		{ States::TryGrabbing, States::Ground, Triggers::EndGrab, nullptr, [this] { handler.changeAnim(animation::ID::MC_idle); }},
		{ States::TryGrabbing, States::Grabbing, Triggers::Grab, nullptr, [this] { handler.changeAnim(animation::ID::MC_idle); }},
		{ States::Grabbing, States::Ground, Triggers::Throw, nullptr, [this] { handler.changeAnim(animation::ID::MC_idle); }},
		{ States::Grabbing, States::Ground, Triggers::EndGrab, nullptr, [this] { handler.changeAnim(animation::ID::MC_idle); }},
	};

	machine.add_transitions(transitions);

	endAnimTriggers = {
		{ animation::ID::MC_roll, Triggers::EndRoll },
		{ animation::ID::MC_light_attack, Triggers::EndLightAttack },
		{ animation::ID::MC_heavy_attack, Triggers::EndHeavyAttack },
		{ animation::ID::MC_grab, Triggers::EndGrab },
		{ animation::ID::MC_roll_attack, Triggers::EndLightAttack },
		{ animation::ID::MC_hurt, Triggers::Recover }
	};

	coords = sf::Vector2f(0, 0);
	AI = ArtificialIntelligence{ hitboxes::Layers::Ally };

	if (init_lens) lens = std::make_shared<Lens>(1000, 1000);
}

ControllableActor::ControllableActor(ActorPipe* pipe, AnimHolder const& holder, std::shared_ptr<Lens> lens) : ControllableActor(pipe, holder, false)
{
	this->lens = lens;
}

std::unique_ptr<Actor> ControllableActor::clone() const
{
	return std::make_unique<ControllableActor>(pipe, handler.getHolder());
}

animation::ID ControllableActor::update(sf::Time const& elapsed, Level const& level)
{
	// button held
	if (holdingRoll) holdRoll += elapsed;

	if (holdingRoll && holdRoll.asMilliseconds() > rollTime) machine.execute(Triggers::HoldSprint);

	// retrieve the animation that ended in the mother class update method
	// and changes the animation to fall if said anim was jumping
	// needed because jumping is not a state and therefore there cannot be a transition from jumping to falling
	auto animEnd = Actor::update(elapsed, level);
	if (animEnd == animation::ID::MC_jump) handler.changeAnim(animation::ID::MC_fall);
	return animEnd;
}

void ControllableActor::pressRoll()
{
	holdRoll = sf::Time::Zero;
	holdingRoll = true;
}

void ControllableActor::releaseRoll()
{
	holdingRoll = false;

	if (holdRoll.asMilliseconds() > rollTime)
	{
		execute(Triggers::ReleaseSprint);
		holdRoll = sf::Time::Zero;
		return;
	}

	execute(Triggers::PressRoll);
}

void ControllableActor::horizontalInput(float dx)
{
	if (machine.state() == States::Grabbing) doThrow(dx, 0);
	else setHorizontalVelocity(dx);
}

void ControllableActor::verticalInput(float dy)
{
	if (dy < 0) machine.execute(Triggers::PressDown);
	else machine.execute(Triggers::ReleaseDown);
}

void ControllableActor::switchWeaponSize()
{
	execute(Triggers::SwitchWeaponSize);
}

void ControllableActor::switchWeaponRange()
{
	execute(Triggers::SwitchWeaponRange);
}

void ControllableActor::pressClone()
{
	if (machine.state() == States::Grabbing && grabbed != nullptr)
	{
		pipe->clonePlayer(this, coords);
	}
	else
	{
		pipe->switchControlled(this);
	}
}

void ControllableActor::setControlled(bool c)
{
	controlled = c;
}

void ControllableActor::switchCurrentLens(float left, float right)
{
	lens->switchCurrentLens(left, right);
}

void ControllableActor::updateControllableChain(ControllableActor* newActor)
{
	newActor->next = next;
	newActor->prev = this;
	next = newActor;
}

ControllableActor* ControllableActor::getNextControllable()
{
	return next;
}

// if the actor is currently controlled by the player, do not take into account the AI's decision
void ControllableActor::takeDecision()
{
	if (controlled) return;
	Actor::takeDecision();
}

bool ControllableActor::seen(LensColors leftLens, LensColors rightLens) const
{
	return true;
}

LensColors ControllableActor::getLeftLens() const
{
	return lens->getLeftLens();
}

LensColors ControllableActor::getRightLens() const
{
	return lens->getRightLens();
}

void ControllableActor::draw(sf::RenderWindow& window, LensColors leftLens, LensColors rightLens) const
{
	Actor::draw(window, leftLens, rightLens);
	lens->draw(window);
}

// returns the attack anim ID depending on the type of attack and weapon
animation::ID ControllableActor::getAttackAnim(bool heavy) const
{
	if (meleeWeapon && bigWeapon && heavy) return animation::ID::MC_heavy_attack;
	if (meleeWeapon && bigWeapon && !heavy) return animation::ID::MC_light_attack;
	if (meleeWeapon && !bigWeapon && heavy) return animation::ID::MC_heavy_attack;
	if (meleeWeapon && !bigWeapon && !heavy) return animation::ID::MC_light_attack;
	if (!meleeWeapon && bigWeapon && heavy) return animation::ID::MC_heavy_attack;
	if (!meleeWeapon && bigWeapon && !heavy) return animation::ID::MC_light_attack;
	if (!meleeWeapon && !bigWeapon && heavy) return animation::ID::MC_heavy_attack;
	//if (!meleeWeapon && !bigWeapon && !heavy)
	return animation::ID::MC_light_attack;
}

void ControllableActor::shoot(bool heavy) const
{
	float xDir = handler.getXDir();
	pipe->writeActor(PrototypesID::PlayerProjectile, coords, xDir * sf::Vector2f{ 1, 0 });
}
