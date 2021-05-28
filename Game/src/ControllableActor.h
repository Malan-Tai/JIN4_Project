#pragma once
#include "Actor.h"

class ControllableActor : public Actor
{
public:
	explicit ControllableActor(AnimHolder const& holder);

	void update(sf::Time const& elapsed) override;

	void pressRoll();
	void releaseRoll();

private:
	int const rollTime = 200;
	bool holdingRoll = false;
	sf::Time holdRoll = sf::Time::Zero;
};