#pragma once
#include "Actor.h"

class ControllableActor : public Actor
{
public:
	explicit ControllableActor(AnimHolder const& holder);

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

	void pressRoll();
	void releaseRoll();
	void pressDown(bool pressed);

private:
	int const rollTime = 200;
	bool holdingRoll = false;
	sf::Time holdRoll = sf::Time::Zero;

	int jumps = 0;
};