#pragma once
#include "Actor.h"
#include <queue>
#include <unordered_map>

enum class PrototypesID
{
	PlayerProjectile
};

class ActorPipe
{
public:
	static ActorPipe& instance();

	void init(AnimHolder const& holder);

	void writeActor(PrototypesID id, sf::Vector2f coords = sf::Vector2f{ 0, 0 }, sf::Vector2f velocity = sf::Vector2f{ 0, 0 });
	std::unique_ptr<Actor> readActor();

private:
	explicit ActorPipe() = default;

	sf::Vector2f normalize(sf::Vector2f v) const;

	std::unordered_map<PrototypesID, std::unique_ptr<Actor>> prototypes;
	std::queue<std::unique_ptr<Actor>> pipe;
};