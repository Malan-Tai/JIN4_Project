#pragma once
#include "ControllableActor.h"
#include <queue>
#include <unordered_map>

// IDs mapped to prototypes in order to clone them easily
enum class PrototypesID
{
	PlayerProjectile
};

// a singleton pipe to communicate between actors and the game loop
// it is used to create actors by writing in it new instaces
// and the game loop reads them and add them to its vector of actors
// it is also used cycle through player character clones
class ActorPipe
{
public:
	explicit ActorPipe(AnimHolder const& holder);

	void writeActor(PrototypesID id, sf::Vector2f coords = sf::Vector2f{ 0, 0 }, sf::Vector2f velocity = sf::Vector2f{ 0, 0 });
	std::unique_ptr<Actor> readActor();

	void clonePlayer(ControllableActor* player, sf::Vector2f coords = sf::Vector2f{ 0, 0 });
	void switchControlled(ControllableActor* player);
	ControllableActor* getNewControlled();

private:
	sf::Vector2f normalize(sf::Vector2f v) const;

	std::unordered_map<PrototypesID, std::unique_ptr<Actor>> prototypes;
	std::queue<std::unique_ptr<Actor>> pipe;

	ControllableActor* newControlled = nullptr;
};