#include "ControllableActor.h"

ControllableActor::ControllableActor(TextureHolder const& holder) : Actor(holder)
{
}

void ControllableActor::update(sf::Time const& elapsed)
{
	if (holdingRoll) holdRoll += elapsed;

	if (holdRoll.asMilliseconds() > rollTime) machine.execute(Triggers::HoldSprint);

	Actor::update(elapsed);
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
