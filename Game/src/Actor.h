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
	LightAttack,
	EndLightAttack,
	HeavyAttack,
	EndHeavyAttack,
	Die
};

class Actor : public Prototype
{
public:
	explicit Actor(AnimHolder const& holder);
	explicit Actor(AnimHolder const& holder, animation::ID id, animation::ID walk);
	explicit Actor(AnimHolder const& holder, animation::ID id, animation::ID walk, float speed);
	Prototype* clone() const override;

	virtual animation::ID update(sf::Time const& elapsed, Level const& level);
	virtual void hits(Actor* other);

	void execute(Triggers trigger);
	void jump();
	void lightAttack();
	void shoot();

	void setCoords(sf::Vector2f c);
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);
	virtual void updateMoveControl();

	void draw(sf::RenderWindow& window) const;
	void changeAnim(animation::ID id);

	bool toRemove() const;
protected:
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
};

using M = FSM::Fsm<States, States::Ground, Triggers>;