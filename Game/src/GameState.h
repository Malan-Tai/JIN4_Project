#pragma once
#include <SFML/Graphics.hpp>

class StateStack;

enum class StatesID
{
	None,
	Playing
};

class GameState
{
public:
	explicit GameState(StateStack* stack);
	virtual ~GameState() = default;

	virtual bool update(sf::Time elapsed) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

	virtual void draw(sf::RenderWindow& window) const = 0;

protected:
	void requestStackPush(StatesID stateID);
	void requestStackPop();
	void requestStateClear();

protected:
	const std::vector<sf::Joystick::Axis> controllerAxes = { sf::Joystick::Axis::X, sf::Joystick::Axis::Y, sf::Joystick::Axis::U, sf::Joystick::Axis::V, sf::Joystick::Axis::PovX, sf::Joystick::Axis::PovY };

private:
	StateStack* stack;
};

