#include "myMain.h"
//#include <iostream>
//#include <unordered_map>
//#include "Level.h"
//#include "ControllableActor.h"
//#include "Command.h"
//#include "ActorPipe.h"
//#include "EnemyActor.h"
//#include "Lens.h"
#include "Application.h"

int myMain()
{
    /*
    sf::RenderWindow window { sf::VideoMode(1000, 1000), "Hollow Lens" };

    sf::Clock gameClock;

    AnimHolder animHolder;
    animHolder.load(animation::ID::MC_idle, "resources/MC_idle");
    animHolder.load(animation::ID::MC_roll, "resources/MC_roll");
    animHolder.load(animation::ID::MC_jump, "resources/MC_jump");
    animHolder.load(animation::ID::MC_fall, "resources/MC_fall");
    animHolder.load(animation::ID::MC_light_attack, "resources/MC_light_attack");
    animHolder.load(animation::ID::MC_heavy_attack, "resources/MC_heavy_attack");
    animHolder.load(animation::ID::MC_grab, "resources/MC_heavy_attack");
    animHolder.load(animation::ID::MC_walk, "resources/MC_walk");
    animHolder.load(animation::ID::MC_roll_attack, "resources/MC_roll_attack");
    animHolder.load(animation::ID::MC_hurt, "resources/MC_hurt");

    animHolder.load(animation::ID::monster_idle, "resources/monster_idle");
    animHolder.load(animation::ID::monster_hurt, "resources/monster_hurt");
    animHolder.load(animation::ID::monster_walk, "resources/monster_walk");
    animHolder.load(animation::ID::fireball, "resources/fireball");
    
    auto pipe = std::make_unique<ActorPipe>(animHolder);

    Level level{};

    auto actors = std::vector<std::unique_ptr<Actor>>{};
    actors.push_back(std::make_unique<ControllableActor>(pipe.get(), animHolder));
    auto controlled = (ControllableActor*)actors[0].get();

    actors.push_back(std::make_unique<EnemyActor>(pipe.get(), animHolder, animation::ID::monster_idle, animation::ID::monster_walk, animation::ID::monster_hurt, 100));

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
    GrabCmd grabcmd{};
    SwitchWeaponRangeCmd rangecmd{};
    SwitchWeaponSizeCmd sizecmd{};
    CloneCmd clonecmd{};
    LeftLensCmd llenscmd{};
    RightLensCmd rlenscmd{};
    HorizontalMoveCmd horizcmd{};
    VerticalMoveCmd vertcmd{};

    // keyboard
    std::unordered_map<sf::Keyboard::Key, Command*> keyboardCmds{};

    keyboardCmds.insert(std::make_pair<sf::Keyboard::Key, Command*>(sf::Keyboard::Space, &jcmd));


    // controller : A, B, X, Y, LB, RB, Select, Start, LJ, RJ + artificial LT, RT
    std::vector<Command*> controllerPressCmds
    {
        &jcmd, &prcmd, &clonecmd, &sizecmd, &rangecmd, &lacmd, nullptr, nullptr, nullptr, nullptr, &grabcmd, &hacmd
    };
    std::vector<Command*> controllerReleaseCmds
    {
        nullptr, &rrcmd,  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
    };

    // controller axes : Left Stick = (X, Y), Right Sitck = (U, V), Directional cross = (PovX, PovY)
    std::vector<sf::Joystick::Axis> controllerAxes = { sf::Joystick::Axis::X, sf::Joystick::Axis::Y, sf::Joystick::Axis::U, sf::Joystick::Axis::V, sf::Joystick::Axis::PovX, sf::Joystick::Axis::PovY };
    std::unordered_map<sf::Joystick::Axis, AxisCommand*> controllerAxesCmds{};

    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, AxisCommand*>(sf::Joystick::Axis::PovX, &horizcmd));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, AxisCommand*>(sf::Joystick::Axis::PovY, &vertcmd));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, AxisCommand*>(sf::Joystick::Axis::Y, &llenscmd));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, AxisCommand*>(sf::Joystick::Axis::V, &rlenscmd));

    float prevZ = 0;

    // game loop
    while (window.isOpen())
    {
        sf::Time elapsed = gameClock.restart();

        // button inputs
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

        // directional inputs
        if (sf::Joystick::isConnected(0))
        {
            for (auto axis : controllerAxes)
            {
                auto found = controllerAxesCmds.find(axis);
                if (found != controllerAxesCmds.end()) found->second->execute(controlled, sf::Joystick::getAxisPosition(0, axis));
            }

            // triggers are directional input in essence but button input in game
            float z = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z); // triggers ( + for left, - for right)
            if (prevZ < 50 && z > 50) // left trigger
            {
                Command* cmd = controllerPressCmds[10];
                if (cmd != nullptr) cmd->execute(controlled);
            }
            if (prevZ > -50 && z < -50) // right trigger
            {
                Command* cmd = controllerPressCmds[11];
                if (cmd != nullptr) cmd->execute(controlled);
            }
            prevZ = z;
        }

        // add created actors
        std::unique_ptr<Actor> added = pipe->readActor();
        while (added != nullptr)
        {
            actors.push_back(std::move(added));
            added = pipe->readActor();
        }

        auto newControlled = pipe->getNewControlled();
        if (newControlled != nullptr)
        {
            controlled->setControlled(false);
            controlled = newControlled;
        }

        LensColors leftLens = controlled->getLeftLens();
        LensColors rightLens = controlled->getRightLens();

        // update
        auto n = actors.size();
        for (int i = 0; i < n; i++)
        {
            auto actor = actors[i].get();
            actor->update(elapsed, level);
        }

        // check hitboxes collisions
        for (int i = 0; i < n; i++)
        {
            auto actor = actors[i].get();
            for (int j = 0; j < n; j++)
            {
                if (i == j) continue;
                auto other = actors[j].get();
                actor->hits(other, leftLens, rightLens);
            }
        }

        actors.erase(std::remove_if(actors.begin(), actors.end(), actorToBeRemoved), actors.end());

        // AIs choose targets
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
                actors[i]->chooseTarget(actorsPtr, leftLens, rightLens);
            }
        }

        // draw
        window.clear(sf::Color::White);

        level.draw(window);

        for (int i = 0; i < actors.size(); i++)
        {
            auto actor = actors[i].get();
            if (actor != controlled) actor->draw(window, leftLens, rightLens);
        }

        controlled->draw(window, leftLens, rightLens);

        window.display();
    }*/

    Application app;
    app.run();

    return 0;
}