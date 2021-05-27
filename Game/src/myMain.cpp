#include "myMain.h"
#include <iostream>
#include "Actor.h"
#include "Command.h"
#include <unordered_map>


int myMain()
{
    sf::RenderWindow window { sf::VideoMode(1000, 1000), "Hollow Lens" };

    sf::Clock gameClock;

    TextureHolder textureHolder;
    textureHolder.load(texture::ID::mainCharacter_idle, "resources/MC_idle.png");

    auto actors = std::vector<std::unique_ptr<Actor>>{};
    actors.push_back(std::make_unique<Actor>(textureHolder));

    Actor* controlled = actors[0].get();

    std::unordered_map<sf::Keyboard::Key, Command*> keyboardCmds{};
    JumpCmd jcmd{};
    keyboardCmds.insert(std::make_pair<sf::Keyboard::Key, Command*>(sf::Keyboard::Space, &jcmd));

    while (window.isOpen())
    {
        sf::Time elapsed = gameClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();

                if (keyboardCmds.find(event.key.code) != keyboardCmds.end()) keyboardCmds.at(event.key.code)->execute(controlled);
            }
        }

        for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            actor->update(elapsed);
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            actor->draw(window);
        }

        window.display();
    }

    return 0;
}