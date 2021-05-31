#include "ActorHitbox.h"

ActorHitbox::ActorHitbox(Hitbox const* hitbox, sf::Vector2f pos, bool inv, float width) : Hitbox(hitbox->getRect(), hitbox->getLayer(), hitbox->getType()), frameWidth(width)
{
	original = hitbox;

	if (!inv) rect = sf::FloatRect(pos.x + rect.left, pos.y + rect.top, rect.width, rect.height);
	else rect = sf::FloatRect(pos.x + width - rect.left - rect.width, pos.y + rect.top, rect.width, rect.height);
}

bool ActorHitbox::intersect(ActorHitbox const* other) const
{
	bool canIntersect = other->type != type &&
		other->type != hitboxes::Type::None && type != hitboxes::Type::None &&
		other->layer != hitboxes::Layers::None && layer != hitboxes::Layers::None &&
		(layer == hitboxes::Layers::All || other->layer == hitboxes::Layers::All ||
			layer != other->layer);

	return canIntersect && rect.intersects(other->rect);
}

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
