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
	Dead
};

enum class Triggers
{
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
	Prototype* clone() const override;

	virtual void update(sf::Time const& elapsed);

	void doJump(); // actually jump
	void jump(); // execute jump trigger
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);

	void draw(sf::RenderWindow& window) const;
	void changeAnim(animation::ID id);

protected:
	float speed = 300;
	float moveControl = 1;

	sf::Vector2f velocity{ 0, 0 };
	sf::Vector2f coords{ 500, 500 };

	//sf::Sprite sprite;
	AnimHandler handler;

	FSM::Fsm<States, States::Ground, Triggers> machine;
	std::map<animation::ID, Triggers> endAnimTriggers;
};

using M = FSM::Fsm<States, States::Ground, Triggers>;