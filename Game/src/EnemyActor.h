#pragma once
#include "Actor.h"

// a base enemy
class EnemyActor : public Actor
{
public:
	explicit EnemyActor(AnimHolder const& holder, animation::ID id, animation::ID walk, animation::ID hurt, float speed);

};