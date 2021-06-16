#pragma once
#include <SFML/Graphics.hpp>
#include "StateStack.h"

class Application
{
public:
	explicit Application();

	void run();

private:
	sf::Clock gameClock;

	sf::RenderWindow window{ sf::VideoMode(1000, 1000), "Hollow Lens" };

	std::unique_ptr<StateStack> stack;
};