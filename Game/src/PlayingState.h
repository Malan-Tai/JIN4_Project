#pragma once
#include <iostream>
#include <unordered_map>
#include "Level.h"
#include "ControllableActor.h"
#include "Command.h"
#include "ActorPipe.h"
#include "GameState.h"

class PlayingState : public GameState
{
public:
	explicit PlayingState(StateStack* stack);

	bool update(sf::Time elapsed) override;
	bool handleEvent(const sf::Event& event) override;

	void draw(sf::RenderWindow& window) const override;

private:
	AnimHolder animHolder;

	std::unique_ptr<ActorPipe> pipe;

	Level level;

	std::vector<std::unique_ptr<Actor>> actors;
	ControllableActor* controlled;

	int const checkTargetTime = 2;
	sf::Time lastCheckedTarget = sf::Time::Zero;

	std::unordered_map<sf::Keyboard::Key, std::unique_ptr<Command>> keyboardCmds;
	std::vector<std::unique_ptr<Command>> controllerPressCmds;
	std::vector<std::unique_ptr<Command>> controllerReleaseCmds;
	std::unordered_map<sf::Joystick::Axis, std::unique_ptr<AxisCommand>> controllerAxesCmds;
	float prevZ = 0;
};
