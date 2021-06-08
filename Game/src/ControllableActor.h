#pragma once
#include "Actor.h"

// the class the player controls :
// either the player's character or their clones
class ControllableActor : public Actor
{
public:
	explicit ControllableActor(AnimHolder const& holder);
	std::unique_ptr<Actor> clone() const override;

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

	void pressRoll();
	void releaseRoll();
	void directionalInput(int dx, int dy);
	void switchWeaponSize();
	void switchWeaponRange();
	void pressClone();
	void setControlled(bool c);

	void updateControllableChain(ControllableActor* newActor);
	ControllableActor* getNextControllable();

	void takeDecision() override;

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
};