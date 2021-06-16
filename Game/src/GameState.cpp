#include "GameState.h"
#include "StateStack.h"

GameState::GameState(StateStack* stack) : stack(stack)
{
}

void GameState::requestStackPush(StatesID stateID)
{
	stack->pushState(stateID);
}

void GameState::requestStackPop()
{
	stack->popState();
}

void GameState::requestStateClear()
{
	stack->clearStates();
}
