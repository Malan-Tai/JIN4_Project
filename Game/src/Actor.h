#pragma once
#include "Prototype.h"
#include "AnimHandler.h"
#include "fsm.h"

enum class States
{
	Ground,
	Fall,
	FastFall,
	FallAttack,
	Roll,
	Sprint,
	LightAttack,
	HeavyAttack,
	GotHit,
	Staggered,
	Dead,
	ToBeRemoved
};

enum class Triggers
{
	None,
	Jump,
	Land,
	Fall,
	PressDown,
	ReleaseDown,
	PressRoll,
	EndRoll,
	HoldSprint,
	ReleaseSprint,
	SwitchWeaponSize,
	SwitchWeaponRange,
	LightAttack,
	EndLightAttack,
	HeavyAttack,
	EndHeavyAttack,
	DoHit,
	GetHit,
	Die,
	Remove
};

class Actor : public Prototype
{
public:
	explicit Actor(AnimHolder const& holder);
	explicit Actor(AnimHolder const& holder, animation::ID id, animation::ID walk);
	explicit Actor(AnimHolder const& holder, animation::ID id, animation::ID walk, float speed);
	std::unique_ptr<Actor> clone() const override;

	virtual animation::ID update(sf::Time const& elapsed, Level const& level);
	virtual void hits(Actor* other);

	void jump();
	void lightAttack();

	void setCoords(sf::Vector2f c);
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);
	virtual void updateMoveControl();

	void draw(sf::RenderWindow& window) const;
	void changeAnim(animation::ID id);

	bool toRemove() const;
protected:
	void execute(Triggers trigger); // use this if you want the trigger to be buffered in case it is not triggered

	float speed = 300;
	float moveControl = 1;

	sf::Vector2f velocity{ 0, 0 };
	sf::Vector2f coords{ 500, 700 };

	AnimHandler handler;
	animation::ID idleAnim;
	animation::ID walkAnim;

	FSM::Fsm<States, States::Ground, Triggers> machine;
	std::map<animation::ID, Triggers> endAnimTriggers;
	Triggers bufferedTrigger = Triggers::None;
	States previousState = States::Ground; // used for chain anim ie roll attack if roll -> ground -> attack
};

using M = FSM::Fsm<States, States::Ground, Triggers>;