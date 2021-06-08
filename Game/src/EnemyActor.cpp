#include "EnemyActor.h"
#include <iostream>

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
		{ States::GotHit, States::Ground, Triggers::Recover, [this] { return hp > 0; }, [this] { handler.changeAnim(idleAnim); }},
		{ States::GotHit, States::ToBeRemoved, Triggers::Recover, [this] { return hp <= 0; }, [this] { handler.changeAnim(idleAnim); }},
		{ States::Ground, States::Staggered, Triggers::Stagger, nullptr, [this] { handler.changeAnim(walkAnim); }},
		{ States::Staggered, States::Ground, Triggers::Recover, [this] { return hp > 0; }, [this] { handler.changeAnim(idleAnim); }},
		{ States::Staggered, States::ToBeRemoved, Triggers::Recover, [this] { return hp <= 0; }, nullptr },
		{ States::Staggered, States::ToBeRemoved, Triggers::GetHit, [this] { return hp <= 0; }, nullptr },

		// grabs
		{ States::Staggered, States::Grabbed, Triggers::Grab, nullptr, [this] { handler.changeAnim(idleAnim); }},
		{ States::Grabbed, States::Ground, Triggers::EndGrab, nullptr, [this] { handler.changeAnim(idleAnim); }},
		{ States::Grabbed, States::Thrown, Triggers::Throw, nullptr, [this] { handler.changeAnim(idleAnim); }},
		{ States::Thrown, States::Ground, Triggers::Land, nullptr, [this] { handler.changeAnim(idleAnim); }},
	};

	machine.add_transitions(transitions);

	endAnimTriggers = {
		{ hurt, Triggers::Recover }
	};
}
