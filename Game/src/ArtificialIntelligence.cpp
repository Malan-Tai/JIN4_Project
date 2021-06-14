#include "ControllableActor.h"
#include <iostream>

ArtificialIntelligence::ArtificialIntelligence(hitboxes::Layers l) : layer(l)
{
}

// chooses the closest actor with a different layer than the AI's
void ArtificialIntelligence::chooseTarget(std::vector<Actor const*>& actors, sf::Vector2f coords, LensColors leftLens, LensColors rightLens)
{
	float minDist = 1000000000;
	target = nullptr;

	for (auto a : actors)
	{
		if (a->getLayer() == layer || !a->seen(leftLens, rightLens)) continue;

		float d = a->distanceTo(coords);
		if (d < minDist)
		{
			minDist = d;
			target = a;
		}
	}
}

// if close enough, attack, else, move towards the target
AI_decision ArtificialIntelligence::makeDecision(sf::Vector2f coords) const
{
	AI_decision decision{ Triggers::None, 0 };
	if (target == nullptr) return decision;

	float d = target->distanceTo(coords);
	if (d < 50)
	{
		decision.trigger = Triggers::LightAttack;
		decision.xDir = 0;
	}
	else
	{
		decision.trigger = Triggers::None;
		float targetX = target->getCoords().x;
		if (targetX < coords.x) decision.xDir = -1;
		else if (targetX > coords.x) decision.xDir = 1;
	}
	return decision;
}

hitboxes::Layers ArtificialIntelligence::getLayer() const
{
	return layer;
}
