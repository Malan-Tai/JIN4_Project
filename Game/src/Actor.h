#pragma once
#include "Prototype.h"
#include "AnimHandler.h"
#include "fsm.h"
#include "Lens.h"

class ActorPipe;

// states allies or enemies can be in
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
	TryGrabbing,
	Grabbing,
	GotHit,
	Staggered,
	Grabbed,
	Thrown,
	Dead,
	ToBeRemoved
};

// triggers to go from one state to another
// some may mean something different depending on the current state
// eg Throw means either do throw or get thrown
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
	Grab,
	EndGrab,
	Throw,
	GetHit,
	Stagger,
	Recover,
	Die,
	Remove
};

// ----------- AI ------------------
// strcuture given to an actor by the AI after it makes a decision
struct AI_decision
{
	Triggers trigger;
	float xDir;
};

// a generic AI
class ArtificialIntelligence
{
public:
	explicit ArtificialIntelligence(hitboxes::Layers l);
	virtual ~ArtificialIntelligence() = default;

	virtual void chooseTarget(std::vector<Actor const*>& actors, sf::Vector2f coords, LensColors leftLens, LensColors rightLens);

	virtual AI_decision makeDecision(sf::Vector2f coords) const;

	hitboxes::Layers getLayer() const;

private:
	Actor const* target = nullptr;
	hitboxes::Layers layer;
};
// ------------------------------------

using M = FSM::Fsm<States, States::Ground, Triggers>;

// any "living" actor ie allies or enemies
class Actor : public Prototype
{
public:
	explicit Actor(ActorPipe* pipe, AnimHolder const& holder);
	explicit Actor(ActorPipe* pipe, AnimHolder const& holder, animation::ID id, animation::ID walk);
	explicit Actor(ActorPipe* pipe, AnimHolder const& holder, animation::ID id, animation::ID walk, float speed);
	std::unique_ptr<Actor> clone() const override;

	virtual animation::ID update(sf::Time const& elapsed, Level const& level);
	void updateFall(sf::Time const& elapsed, Level const& level);
	void updateTimers(sf::Time const& elapsed);
	void updateRegen(sf::Time const& elapsed);
	virtual void hits(Actor* other, LensColors leftLens, LensColors rightLens);

	void spendStamina(float cost);

	void jump();
	void lightAttack();
	void heavyAttack();
	void grab();
	void doThrow(float dx, float dy);
	void getThrown(float dx, float dy);
	void getHit(float dmg, float poiseDmg);

	void setCoords(sf::Vector2f c);
	void setVelocity(sf::Vector2f unitVelocity);
	void setHorizontalVelocity(float dx);
	virtual void updateMoveControl();

	virtual void draw(sf::RenderWindow& window, LensColors leftLens, LensColors rightLens, bool showBar = true) const;
	void changeAnim(animation::ID id);

	virtual bool seen(LensColors leftLens, LensColors rightLens) const;

	bool toRemove() const;

	float distanceTo(sf::Vector2f point) const;
	sf::Vector2f const& getCoords() const;

	void chooseTarget(std::vector<Actor const*>& actors, LensColors leftLens, LensColors rightLens);
	virtual void takeDecision();
	hitboxes::Layers getLayer() const;

#if TESTS
	M const& getMachine() const;
	sf::Vector2f getVelocity() const;
	Triggers getBuffer() const;
#endif

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

	float maxHP = 50;
	float hp = maxHP;
	float fantomHP = hp;
	float const fantomDecay = 5;

	float strength = 3;

	float maxStamina = 50;
	float stamina = maxStamina;
	float const staminaRegen = 10;
	int const noStamRegenTime = 1500;
	sf::Time noStamRegen = sf::Time::Zero;
	float fantomStamina = stamina;

	float stabilityMaxHP = 100;
	float stabilityHP = stabilityMaxHP;
	float const stabilityRegen = 3;
	float const cancelledStabilityDamage = 50;

	int const staggerMaxTime = 5000;
	sf::Time staggerTime = sf::Time::Zero;

	int const grabMaxTime = 1500;
	sf::Time grabTime = sf::Time::Zero;
	Actor* grabbed = nullptr;

	LensColors lensColor = LensColors::None;

	ActorPipe* pipe = nullptr;
};