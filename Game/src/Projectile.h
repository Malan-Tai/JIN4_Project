#pragma once
#include "Actor.h"

class Projectile : public Actor
{
public:
	explicit Projectile(AnimHolder const& holder, animation::ID moveAnim, float speed, float maxDistance = 300, bool piercing = false);
	Prototype* clone() const override;

	animation::ID update(sf::Time const& elapsed, Level const& level) override;

private:
	float const maxDist = 300;
	float curDist = 0;
	bool const piercing = false;
};
