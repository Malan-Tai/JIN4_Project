#include "myMain.h"
#include <iostream>
#include <pugixml.hpp>
#include <SFML/Graphics.hpp>


int myMain()
{
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1000, 1000), "Hollow Lens");

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window->close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window->close();
            }
        }

        window->clear(sf::Color::White);
        window->display();
    }

    return 0;
}