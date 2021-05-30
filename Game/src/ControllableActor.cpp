#include "ControllableActor.h"

ControllableActor::ControllableActor(AnimHolder const& holder) : Actor(holder)
{
	std::vector<M::Trans> transitions
	{
		// from state     , to state      , trigger,    guard   , action
		{ States::Ground, States::Fall, Triggers::Jump, nullptr, [this] { velocity = sf::Vector2f{ 0, -500 }; handler.changeAnim(animation::ID::MC_jump); }},
		{ States::Ground, States::Roll, Triggers::PressRoll, nullptr, [this] { this->changeAnim(animation::ID::MC_roll); }},
		{ States::Fall, States::Roll, Triggers::PressRoll, nullptr, [this] { this->changeAnim(animation::ID::MC_roll); }},
		{ States::Roll, States::Ground, Triggers::EndRoll, nullptr, [this] { this->changeAnim(animation::ID::MC_idle); }},
		{ States::Ground, States::Sprint, Triggers::HoldSprint, nullptr, nullptr },
		{ States::Sprint, States::Ground, Triggers::ReleaseSprint, nullptr, nullptr },
		{ States::Ground, States::LightAttack, Triggers::LightAttack, nullptr, [this] { this->changeAnim(animation::ID::MC_attack); }},
		{ States::LightAttack, States::Ground, Triggers::EndLightAttack, nullptr, [this] { this->changeAnim(animation::ID::MC_idle); }},
	};

	machine.add_transitions(transitions);

	endAnimTriggers = {
		{ animation::ID::MC_roll, Triggers::EndRoll },
		{ animation::ID::MC_attack, Triggers::EndLightAttack }
	};
}

animation::ID ControllableActor::update(sf::Time const& elapsed)
{
	if (holdingRoll) holdRoll += elapsed;

	if (holdRoll.asMilliseconds() > rollTime) machine.execute(Triggers::HoldSprint);

	auto animEnd = Actor::update(elapsed);
	if (animEnd == animation::ID::MC_jump) handler.changeAnim(animation::ID::MC_fall);
	return animEnd;
}

void ControllableActor::pressRoll()
{
	holdRoll = sf::Time::Zero;
	holdingRoll = true;
}

void ControllableActor::releaseRoll()
{
	holdingRoll = false;

	if (machine.state() == States::Sprint)
	{
		machine.execute(Triggers::ReleaseSprint);
		holdRoll = sf::Time::Zero;
		return;
	}

	if (holdRoll.asMilliseconds() < rollTime)
	{
		machine.execute(Triggers::PressRoll);
	}
}
