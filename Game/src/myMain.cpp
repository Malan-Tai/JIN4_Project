#include "myMain.h"
#include <iostream>
#include "Actor.h"


int myMain()
{
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1000, 1000), "Hollow Lens");

    sf::Clock gameClock;

    auto actors = std::vector<std::unique_ptr<Actor>>{};

    std::unique_ptr<TextureHolder> textureHolder;
    textureHolder->load(texture::ID::mainCharacter_idle, "pouet");

    while (window->isOpen())
    {
        sf::Time elapsed = gameClock.restart();

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

        /*for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            actor->draw(window.get());
        }*/

        window->display();
    }

    return 0;
}