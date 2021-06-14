#pragma once
#include "Actor.h"

// the class the player controls :
// either the player's character or their clones
class ControllableActor : public Actor
{
public:
	explicit ControllableActor(ActorPipe* pipe, AnimHolder const& holder, bool init_lens = true);
	explicit ControllableActor(ActorPipe* pipe, AnimHolder const& holder, std::shared_ptr<Lens> lens);
	std::unique_ptr<Actor> clone() const override;

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

	void pressRoll();
	void releaseRoll();
	void horizontalInput(float dx);
	void verticalInput(float dy);
	void switchWeaponSize();
	void switchWeaponRange();
	void pressClone();
	void setControlled(bool c);
	void switchCurrentLens(float left, float right);

	void updateControllableChain(ControllableActor* newActor);
	ControllableActor* getNextControllable();

	void takeDecision() override;

	bool seen(LensColors leftLens, LensColors rightLens) const override;
	LensColors getLeftLens() const;
	LensColors getRightLens() const;

	void draw(sf::RenderWindow& window, LensColors leftLens, LensColors rightLens) const override;

private:
	animation::ID getAttackAnim(bool heavy) const;
	void shoot(bool heavy) const;

	int const rollTime = 200;
	bool holdingRoll = false;
	sf::Time holdRoll = sf::Time::Zero;

	int jumps = 0;
	int airRolls = 0;

	// weapons
	bool meleeWeapon = true;
	bool bigWeapon = false;

	// double chained list : used to cycle through controllable clones
	// have to be updated on death : prev.next = next ; next.prev = prev;
	ControllableActor* prev = nullptr;
	ControllableActor* next = nullptr;

	bool controlled = true;

	std::shared_ptr<Lens> lens = nullptr;
};