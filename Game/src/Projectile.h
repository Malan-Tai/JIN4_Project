#pragma once
#include "Actor.h"

// a projectile which travels in a given direction until
// it hits something if it isn't piercing,
// it reaches its max travel distance
// or it reaches its animation end
class Projectile : public Actor
{
public:
	explicit Projectile(ActorPipe* pipe, AnimHolder const& holder, animation::ID moveAnim, float speed, float maxDistance = 300, bool piercing = false);
	std::unique_ptr<Actor> clone() const override;

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

	void takeDecision() override;

private:
	float const maxDist = 300;
	float curDist = 0;
	bool const piercing = false;
};
