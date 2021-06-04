#pragma once
#include "Actor.h"

class ControllableActor : public Actor
{
public:
	explicit ControllableActor(AnimHolder const& holder);
	std::unique_ptr<Actor> clone() const override;

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

	void pressRoll();
	void releaseRoll();
	void pressDown(bool pressed);
	void switchWeaponSize();
	void switchWeaponRange();
	void pressClone();
	void releaseClone();

	void updateControllableChain(ControllableActor* newActor);
	ControllableActor* getNextControllable();

private:
	animation::ID getAttackAnim(bool heavy) const;
	void shoot(bool heavy) const;

	int const rollTime = 200;
	bool holdingRoll = false;
	sf::Time holdRoll = sf::Time::Zero;

	int jumps = 0;

	// weapons
	bool meleeWeapon = true;
	bool bigWeapon = false;

	// have to be updated on death : prev.next = next ; next.prev = prev;
	ControllableActor* prev = nullptr;
	ControllableActor* next = nullptr;

	int const cloneTime = 700;
	int const switchCloneTime = 200;
	bool holdingClone = false;
	sf::Time holdClone = sf::Time::Zero;
};