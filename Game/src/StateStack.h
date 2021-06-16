#pragma once
#include "GameState.h"

enum class Action
{
	Push,
	Pop,
	Clear,
};

struct PendingChange
{
	explicit PendingChange(Action action, StatesID stateID = StatesID::None);

	Action action;
	StatesID stateID;
};

class StateStack : private sf::NonCopyable
{
public:
	explicit StateStack() = default;

	void update(sf::Time elapsed);
	void handleEvent(const sf::Event& event);

	void draw(sf::RenderWindow& window) const;

	void pushState(StatesID stateID);
	void popState();
	void clearStates();

private:
	std::unique_ptr<GameState> createState(StatesID stateID);
	void applyPendingChanges();

	std::vector<std::unique_ptr<GameState>> stack;
	std::vector<PendingChange> pending;
};