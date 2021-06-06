#include "EnemyActor.h"

EnemyActor::EnemyActor(AnimHolder const& holder, animation::ID id, animation::ID walk, animation::ID hurt, float speed) : Actor(holder, id, walk, speed)
{
	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		// air
		{ States::Ground, States::Fall, Triggers::Fall, nullptr, nullptr},
		{ States::Fall, States::Ground, Triggers::Land, nullptr, [this, id] { velocity = sf::Vector2f{ 0, 0 }; handler.changeAnim(id); }},

		// hits
		{ States::Ground, States::GotHit, Triggers::GetHit, nullptr, [this, hurt] { velocity = sf::Vector2f{ 0, 0 }; handler.changeAnim(hurt); }},
		{ States::GotHit, States::Ground, Triggers::Recover, nullptr, [this] { handler.changeAnim(idleAnim); }},
	};

	machine.add_transitions(transitions);

	endAnimTriggers = {
		{ hurt, Triggers::Recover }
	};
}
