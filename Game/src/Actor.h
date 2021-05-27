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
	LightAttack,
	EndLightAttack,
	HeavyAttack,
	EndHeavyAttack,
	Die
};

class Actor : public Prototype
{
public:
	explicit Actor(TextureHolder const& holder);
	Prototype* clone() const override;

	void update(sf::Time const& elapsed);

	void jump();
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);

	void draw(sf::RenderWindow& window) const;

private:
	float speed = 300;
	float moveControl = 1;

	sf::Vector2f velocity{ 0, 0 };
	sf::Vector2f coords{ 500, 500 };

	sf::Sprite sprite;

	TextureHolder const& holder;

	FSM::Fsm<States, States::Ground, Triggers> machine;
};

using M = FSM::Fsm<States, States::Ground, Triggers>;