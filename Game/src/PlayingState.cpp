#include "PlayingState.h"
#include "EnemyActor.h"

PlayingState::PlayingState(StateStack* stack) : GameState(stack)
{
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

    pipe = std::make_unique<ActorPipe>(animHolder);

    actors.push_back(std::make_unique<ControllableActor>(pipe.get(), animHolder));
    controlled = (ControllableActor*)actors[0].get();

    actors.push_back(std::make_unique<EnemyActor>(pipe.get(), animHolder, animation::ID::monster_idle, animation::ID::monster_walk, animation::ID::monster_hurt, 100));

    // INPUTS

    // keyboard
    keyboardCmds.insert(std::make_pair<sf::Keyboard::Key, std::unique_ptr<Command>>(sf::Keyboard::Space, std::make_unique<JumpCmd>()));

    // controller : A, B, X, Y, LB, RB, Select, Start, LJ, RJ + artificial LT, RT
    controllerPressCmds.push_back(std::make_unique<JumpCmd>());                 // A
    controllerPressCmds.push_back(std::make_unique<PressRollCmd>());            // B
    controllerPressCmds.push_back(std::make_unique<CloneCmd>());                // X
    controllerPressCmds.push_back(std::make_unique<SwitchWeaponSizeCmd>());     // Y
    controllerPressCmds.push_back(std::make_unique<SwitchWeaponRangeCmd>());    // LB
    controllerPressCmds.push_back(std::make_unique<LightAttackCmd>());          // RB
    controllerPressCmds.push_back(nullptr);                                     // Select
    controllerPressCmds.push_back(nullptr);                                     // Start
    controllerPressCmds.push_back(nullptr);                                     // LJ
    controllerPressCmds.push_back(nullptr);                                     // RJ
    controllerPressCmds.push_back(std::make_unique<GrabCmd>());                 // LT
    controllerPressCmds.push_back(std::make_unique<HeavyAttackCmd>());          // RT

    controllerReleaseCmds.push_back(nullptr);                                   // A
    controllerReleaseCmds.push_back(std::make_unique<ReleaseRollCmd>());        // B
    controllerReleaseCmds.push_back(nullptr);                                   // X
    controllerReleaseCmds.push_back(nullptr);                                   // Y
    controllerReleaseCmds.push_back(nullptr);                                   // LB
    controllerReleaseCmds.push_back(nullptr);                                   // RB
    controllerReleaseCmds.push_back(nullptr);                                   // Select
    controllerReleaseCmds.push_back(nullptr);                                   // Start
    controllerReleaseCmds.push_back(nullptr);                                   // LJ
    controllerReleaseCmds.push_back(nullptr);                                   // RJ
    controllerReleaseCmds.push_back(nullptr);                                   // LT
    controllerReleaseCmds.push_back(nullptr);                                   // RT

    // controller axes : Left Stick = (X, Y), Right Sitck = (U, V), Directional cross = (PovX, PovY)
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, std::unique_ptr<AxisCommand>>(sf::Joystick::Axis::PovX, std::make_unique<HorizontalMoveCmd>()));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, std::unique_ptr<AxisCommand>>(sf::Joystick::Axis::PovY, std::make_unique<VerticalMoveCmd>()));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, std::unique_ptr<AxisCommand>>(sf::Joystick::Axis::Y, std::make_unique<LeftLensCmd>()));
    controllerAxesCmds.insert(std::make_pair<sf::Joystick::Axis, std::unique_ptr<AxisCommand>>(sf::Joystick::Axis::V, std::make_unique<RightLensCmd>()));
}

bool PlayingState::update(sf::Time elapsed)
{
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
        if (prevZ < 50 && z > 50) // left trigger press
        {
            if (controllerPressCmds[10] != nullptr) controllerPressCmds[10]->execute(controlled);
        }
        if (prevZ > 50 && z < 50) // left trigger release
        {
            if (controllerReleaseCmds[10] != nullptr) controllerReleaseCmds[10]->execute(controlled);
        }
        if (prevZ > -50 && z < -50) // right trigger press
        {
            if (controllerPressCmds[11] != nullptr) controllerPressCmds[11]->execute(controlled);
        }
        if (prevZ < -50 && z > -50) // right trigger release
        {
            if (controllerReleaseCmds[11] != nullptr) controllerReleaseCmds[11]->execute(controlled);
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

    auto actorToBeRemoved = [](const std::unique_ptr<Actor>& a) { return a->toRemove(); };
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

    return false;
}

bool PlayingState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = keyboardCmds.find(event.key.code);
        if (found != keyboardCmds.end()) found->second->execute(controlled);
    }
    else if (event.type == sf::Event::JoystickButtonPressed)
    {
        if (controllerPressCmds[event.joystickButton.button] != nullptr) controllerPressCmds[event.joystickButton.button]->execute(controlled);
    }
    else if (event.type == sf::Event::JoystickButtonReleased)
    {
        if (controllerReleaseCmds[event.joystickButton.button] != nullptr) controllerReleaseCmds[event.joystickButton.button]->execute(controlled);
    }
    return false;
}

void PlayingState::draw(sf::RenderWindow& window) const
{
    LensColors leftLens = controlled->getLeftLens();
    LensColors rightLens = controlled->getRightLens();

    level.draw(window);

    for (int i = 0; i < actors.size(); i++)
    {
        auto actor = actors[i].get();
        if (actor != controlled) actor->draw(window, leftLens, rightLens);
    }

    controlled->draw(window, leftLens, rightLens);
}
