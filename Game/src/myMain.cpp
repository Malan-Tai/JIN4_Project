#include "myMain.h"
#include <iostream>
#include <unordered_map>
#include "Level.h"
#include "ControllableActor.h"
#include "Command.h"
#include "ActorPipe.h"

int myMain()
{
    sf::RenderWindow window { sf::VideoMode(1000, 1000), "Hollow Lens" };

    sf::Clock gameClock;

    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_roll, "resources/MC_roll");
    animHolder.load(animation::ID::MC_jump, "resources/MC_jump");
    animHolder.load(animation::ID::MC_fall, "resources/MC_fall");
    animHolder.load(animation::ID::MC_light_attack, "resources/MC_light_attack");
    animHolder.load(animation::ID::MC_heavy_attack, "resources/MC_heavy_attack");
    animHolder.load(animation::ID::MC_walk, "resources/MC_walk");
    animHolder.load(animation::ID::MC_roll_attack, "resources/MC_roll_attack");

    animHolder.load(animation::ID::monster_idle, "resources/monster_idle");
    animHolder.load(animation::ID::fireball, "resources/fireball");

    ActorPipe::instance().init(animHolder);

    Level level{};

    auto actors = std::vector<std::unique_ptr<Actor>>{};
    actors.push_back(std::make_unique<ControllableActor>(animHolder));
    auto controlled = (ControllableActor*)actors[0].get();

    actors.push_back(std::make_unique<Actor>(animHolder, animation::ID::monster_idle, animation::ID::monster_idle));

    auto actorToBeRemoved = [](const std::unique_ptr<Actor>& a) { return a->toRemove(); };

    // AI
    int const checkTargetTime = 2;
    sf::Time lastCheckedTarget = sf::Time::Zero;

    // INPUTS
    JumpCmd jcmd{};
    PressRollCmd prcmd{};
    ReleaseRollCmd rrcmd{};
    LightAttackCmd lacmd{};
    HeavyAttackCmd hacmd{};
    SwitchWeaponRangeCmd rangecmd{};
    SwitchWeaponSizeCmd sizecmd{};
    PressCloneCmd pccmd{};
    ReleaseCloneCmd rccmd{};

    // keyboard
    std::unordered_map<sf::Keyboard::Key, Command*> keyboardCmds{};

    keyboardCmds.insert(std::make_pair<sf::Keyboard::Key, Command*>(sf::Keyboard::Space, &jcmd));


    // controller : A, B, X, Y, LB, RB, Select, Start, LJ, RJ + artificial LT, RT
    std::vector<Command*> controllerPressCmds
    {
        &jcmd, &prcmd,  &pccmd, &sizecmd, &rangecmd, &lacmd, nullptr, nullptr, nullptr, nullptr, nullptr, &hacmd
    };
    std::vector<Command*> controllerReleaseCmds
    {
        nullptr, &rrcmd,  &rccmd, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };
    float prevZ = 0;

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

            bool pressedDown = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) < -50;
            controlled->pressDown(pressedDown);

            float z = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z); // triggers ( + for left, - for right)
            if (prevZ < 50 && z > 50) // left trigger
            {
                Command* cmd = controllerReleaseCmds[10];
                if (cmd != nullptr) cmd->execute(controlled);
            }
            if (prevZ > -50 && z < -50) // right trigger
            {
                Command* cmd = controllerPressCmds[11];
                if (cmd != nullptr) cmd->execute(controlled);
            }
            prevZ = z;
        }

        std::unique_ptr<Actor> added = ActorPipe::instance().readActor();
        while (added != nullptr)
        {
            actors.push_back(std::move(added));
            added = ActorPipe::instance().readActor();
        }

        auto newControlled = ActorPipe::instance().getNewControlled();
        if (newControlled != nullptr)
        {
            controlled->setControlled(false);
            controlled = newControlled;
        }

        auto n = actors.size();
        for (int i = 0; i < n; i++)
        {
            auto actor = actors[i].get();
            actor->update(elapsed, level);
        }

        for (int i = 0; i < n; i++)
        {
            auto actor = actors[i].get();
            for (int j = 0; j < n; j++)
            {
                if (i == j) continue;
                auto other = actors[j].get();
                actor->hits(other);
            }
        }

        actors.erase(std::remove_if(actors.begin(), actors.end(), actorToBeRemoved), actors.end());

        lastCheckedTarget -= elapsed;
        if (lastCheckedTarget.asSeconds() <= 0)
        {
            lastCheckedTarget = sf::seconds(checkTargetTime);

            std::vector<Actor const*> actorsPtr{};
            for (int i = 0; i < n; i++)
            {
                actorsPtr.push_back(actors[i].get());
            }

            for (int i = 0; i < n; i++)
            {
                actors[i]->chooseTarget(actorsPtr);
            }
        }


        window.clear(sf::Color::White);

        level.draw(window);

        for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            actor->draw(window);
        }

        window.display();
    }

    return 0;
}