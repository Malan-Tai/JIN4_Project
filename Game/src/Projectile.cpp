#include "Projectile.h"
#include <iostream>

Projectile::Projectile(AnimHolder const& holder, animation::ID moveAnim, float speed, float maxDistance, bool piercing) : Actor(holder, moveAnim, moveAnim, speed), maxDist(maxDistance), piercing(piercing)
{
	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		{ States::Ground, States::ToBeRemoved, Triggers::Remove, nullptr, nullptr },
	};
	if (!piercing) transitions.push_back(
		{ States::Ground, States::ToBeRemoved, Triggers::DoHit, nullptr, nullptr });

	machine.add_transitions(transitions);

	endAnimTriggers = {
		{ moveAnim, Triggers::Remove }, // remove the projectile if the anim ends
	};
}

std::unique_ptr<Actor> Projectile::clone() const
{
	return std::make_unique<Projectile>(handler.getHolder(), walkAnim, speed, maxDist, piercing);
}

animation::ID Projectile::update(sf::Time const& elapsed, Level const& level)
{
	curDist += speed * elapsed.asSeconds();
	if (curDist >= maxDist) machine.execute(Triggers::Remove);

	return Actor::update(elapsed, level);
}
