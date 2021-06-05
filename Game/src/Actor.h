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
	RollLightAttack,
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

// ----------- AI ------------------
struct AI_decision
{
	Triggers trigger;
	float xDir;
};

class ArtificialIntelligence
{
public:
	explicit ArtificialIntelligence(hitboxes::Layers l);
	virtual ~ArtificialIntelligence() = default;

	virtual void chooseTarget(std::vector<Actor const*>& actors, sf::Vector2f coords);

	virtual AI_decision makeDecision(sf::Vector2f coords) const;

	hitboxes::Layers getLayer() const;

private:
	Actor const* target = nullptr;
	hitboxes::Layers layer;
};
// ------------------------------------

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
	void heavyAttack();

	void setCoords(sf::Vector2f c);
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);
	virtual void updateMoveControl();

	void draw(sf::RenderWindow& window) const;
	void changeAnim(animation::ID id);

	bool toRemove() const;

	float distanceTo(sf::Vector2f point) const;
	sf::Vector2f const& getCoords() const;

	void chooseTarget(std::vector<Actor const*>& actors);
	virtual void takeDecision();
	hitboxes::Layers getLayer() const;

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
	int const forgetPrevStateTime = 500;
	sf::Time forgetPrevState = sf::Time::Zero;

	ArtificialIntelligence AI;
};

using M = FSM::Fsm<States, States::Ground, Triggers>;