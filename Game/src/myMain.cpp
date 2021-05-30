#include "myMain.h"
#include <iostream>
#include "ControllableActor.h"
#include "Command.h"
#include <unordered_map>

// enum class ControllerBtn { A, B, X, Y, LB, RB, Select, Start, LJ, RJ, };

int myMain()
{
    sf::RenderWindow window { sf::VideoMode(1000, 1000), "Hollow Lens" };

    sf::Clock gameClock;

    TextureHolder textureHolder;
    textureHolder.load(texture::ID::MC_idle, "resources/MC_idle.png");

    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_roll, "resources/MC_roll");

    auto actors = std::vector<std::unique_ptr<Actor>>{};
    actors.push_back(std::make_unique<ControllableActor>(animHolder));

    auto controlled = actors[0].get();

    // INPUTS
    JumpCmd jcmd{};
    PressRollCmd prcmd{};
    ReleaseRollCmd rrcmd{};

    // keyboard
    std::unordered_map<sf::Keyboard::Key, Command*> keyboardCmds{};

    keyboardCmds.insert(std::make_pair<sf::Keyboard::Key, Command*>(sf::Keyboard::Space, &jcmd));


    // controller
    std::vector<Command*> controllerPressCmds
    {
        &jcmd, &prcmd,  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    };
    std::vector<Command*> controllerReleaseCmds
    {
        nullptr, &rrcmd,  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    };

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

                auto found = keyboardCmds.find(event.key.code);
                if (found != keyboardCmds.end()) found->second->execute(controlled);
            }
            else if (event.type == sf::Event::JoystickButtonPressed)
            {
                if (event.joystickButton.button == 7) window.close();

                Command* cmd = controllerPressCmds[event.joystickButton.button];
                if (cmd != nullptr) cmd->execute(controlled);
            }
            else if (event.type == sf::Event::JoystickButtonReleased)
            {
                Command* cmd = controllerReleaseCmds[event.joystickButton.button];
                if (cmd != nullptr) cmd->execute(controlled);
            }
        }

        if (sf::Joystick::isConnected(0))
        {
            float dx = 0;
            float controllerDx = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);

            if (controllerDx >= 50) dx = 1;
            if (controllerDx <= -50) dx = -1;

            controlled->setHorizontalVelocity(dx);
        }

        /*std::vector<Hitbox> hitboxes{};
        std::vector<Hitbox> hurtboxes{};*/
        for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            actor->update(elapsed);
            //actor->addHitboxes(hitboxes, hurtboxes);
        }

        /*for (auto const& atkbox : hurtboxes)
        {
            for (auto)
        }*/

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