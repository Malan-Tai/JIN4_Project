#include "ActorPipe.h"

ActorPipe::ActorPipe()
{
	prototypes = {
		
	};
}

ActorPipe& ActorPipe::instance()
{
	static auto instance = new ActorPipe();
	return *instance;
}

void ActorPipe::writeActor(PrototypesID id, sf::Vector2f coords, sf::Vector2f velocity)
{
	auto found = prototypes.find(id);
	if (found == prototypes.end()) return;

	auto clone = (Actor*)found->second->clone();
	clone->setVelocity(normalize(velocity));
	clone->setCoords(coords);

	pipe.push(clone);
}

std::unique_ptr<Actor> ActorPipe::readActor()
{
	if (pipe.size() <= 0) return nullptr;
	
	auto res = pipe.front();
	pipe.pop();
	return std::unique_ptr<Actor>(res);
}

sf::Vector2f ActorPipe::normalize(sf::Vector2f v)
{
	if (v.x == 0 && v.y == 0) return v;
	float len = sqrt(v.x * v.x + v.y * v.y);
	return v / len;
}
