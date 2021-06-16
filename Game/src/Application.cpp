#include "Application.h"

Application::Application()
{
	stack = std::make_unique<StateStack>();
    stack->pushState(StatesID::Playing);
}

void Application::run()
{
    while (window.isOpen())
    {
        sf::Time elapsed = gameClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else stack->handleEvent(event);
        }

        stack->update(elapsed);

        window.clear(sf::Color::White);
        stack->draw(window);
        window.display();
    }
}
