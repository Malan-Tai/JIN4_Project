#include "ActorHitbox.h"

ActorHitbox::ActorHitbox(Hitbox const* hitbox, sf::Vector2f pos, bool inv, float width) : original(hitbox), frameWidth(width)
{
	if (!inv) rect = sf::FloatRect(pos.x + rect.left, pos.y + rect.top, rect.width, rect.height);
	else rect = sf::FloatRect(pos.x + width - rect.left - rect.width, pos.y + rect.top, rect.width, rect.height);
}

// returns whether thiss hitbox intersects the other one
bool ActorHitbox::intersect(ActorHitbox const* other) const
{
	bool canIntersect = other->getType() != original->getType() &&
		other->getType() != hitboxes::Type::None && original->getType() != hitboxes::Type::None &&
		other->getLayer() != hitboxes::Layers::None && original->getLayer() != hitboxes::Layers::None &&
		(original->getLayer() == hitboxes::Layers::All || other->getLayer() == hitboxes::Layers::All ||
			original->getLayer() != other->getLayer());

	return canIntersect && rect.intersects(other->rect);
}

// returns whether this hitbox collides with the platform's rectangle
float ActorHitbox::collides(sf::FloatRect const& _rect) const
{
	if (!rect.intersects(_rect)) return 0;											// if doesn't collide, do not move actor
	if (_rect.top <= rect.top && rect.top <= _rect.top + _rect.height)				// if actor's top is inside the platform, move downwards
		return _rect.top + _rect.height - rect.top;
	return _rect.top - rect.top - rect.height;										// else move upwards
}

void ActorHitbox::setPosition(sf::Vector2f pos, bool inv)
{
	auto ogRect = original->getRect();
	if (!inv) rect = sf::FloatRect(pos.x + ogRect.left, pos.y + ogRect.top, ogRect.width, ogRect.height);
	else rect = sf::FloatRect(pos.x + frameWidth - ogRect.left - ogRect.width, pos.y + ogRect.top, ogRect.width, ogRect.height);
}

hitboxes::Type ActorHitbox::getType() const
{
	return original->getType();
}

hitboxes::Layers ActorHitbox::getLayer() const
{
	return original->getLayer();
}
