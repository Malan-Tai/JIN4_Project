#include "StateStack.h"
#include "PlayingState.h"

PendingChange::PendingChange(Action action, StatesID stateID) : action(action), stateID(stateID)
{
}

void StateStack::update(sf::Time elapsed)
{
	auto n = stack.size();
	for (int i = 0; i < n; i++)
	{
		if (stack[i]->update(elapsed)) break;
	}

	applyPendingChanges();
}

void StateStack::handleEvent(const sf::Event& event)
{
	auto n = stack.size();
	for (int i = 0; i < n; i++)
	{
		if (stack[i]->handleEvent(event)) break;
	}

	applyPendingChanges();
}

void StateStack::draw(sf::RenderWindow& window) const
{
	auto n = stack.size();
	for (int i = 0; i < n; i++)
	{
		stack[i]->draw(window);
	}
}

void StateStack::pushState(StatesID stateID)
{
	pending.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	pending.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	pending.push_back(PendingChange(Action::Clear));
}

std::unique_ptr<GameState> StateStack::createState(StatesID stateID)
{
	switch (stateID)
	{
	case StatesID::Playing:
		return std::make_unique<PlayingState>(this);

	default:
		return nullptr;
	}
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : pending)
	{
		switch (change.action)
		{
		case Action::Push:
			stack.push_back(createState(change.stateID));
			break;

		case Action::Pop:
			stack.pop_back();
			break;

		case Action::Clear:
			stack.clear();
			break;
		}
	}

	pending.clear();
}

