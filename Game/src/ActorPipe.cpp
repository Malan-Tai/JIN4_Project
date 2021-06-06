#include "ActorPipe.h"
#include "Projectile.h"
#include <iostream>

ActorPipe& ActorPipe::instance()
{
	static auto instance = new ActorPipe();
	return *instance;
}

void ActorPipe::init(AnimHolder const& holder)
{
	prototypes.emplace(PrototypesID::PlayerProjectile, std::make_unique<Projectile>(holder, animation::ID::fireball, 600.f));

}

void ActorPipe::writeActor(PrototypesID id, sf::Vector2f coords, sf::Vector2f velocity)
{
	auto found = prototypes.find(id);
	if (found == prototypes.end()) return;

	auto clone = found->second->clone();
	clone->setVelocity(normalize(velocity));
	clone->setCoords(coords);

	pipe.push(std::move(clone));
}

std::unique_ptr<Actor> ActorPipe::readActor()
{
	if (pipe.size() <= 0) return nullptr;
	
	auto res = std::move(pipe.front());
	pipe.pop();
	return res;
}

void ActorPipe::clonePlayer(ControllableActor* player, sf::Vector2f coords)
{
	auto clone = player->clone();
	player->updateControllableChain((ControllableActor*)clone.get());
	clone->setCoords(coords);
	((ControllableActor*)clone.get())->setControlled(false);

	pipe.push(std::move(clone));
}

void ActorPipe::switchControlled(ControllableActor* player)
{
	newControlled = player->getNextControllable();
}

ControllableActor* ActorPipe::getNewControlled()
{
	auto ptr = newControlled;
	if (ptr != nullptr)
	{
		newControlled->setControlled(true);
		newControlled = nullptr;
	}
	return ptr;
}

sf::Vector2f ActorPipe::normalize(sf::Vector2f v) const
{
	if (v.x == 0 && v.y == 0) return v;
	float len = sqrt(v.x * v.x + v.y * v.y);
	return v / len;
}
