#include "ActorHitbox.h"

ActorHitbox::ActorHitbox(Hitbox const* hitbox, sf::Vector2f pos, bool inv, float width) : Hitbox(hitbox->getRect(), hitbox->getLayer(), hitbox->getType()), frameWidth(width)
{
	original = hitbox;

	if (!inv) rect = sf::FloatRect(pos.x + rect.left, pos.y + rect.top, rect.width, rect.height);
	else rect = sf::FloatRect(pos.x + width - rect.left - rect.width, pos.y + rect.top, rect.width, rect.height);
}

bool ActorHitbox::intersect(ActorHitbox const& other) const
{
	bool canIntersect = other.type != type &&
		other.type != hitboxes::Type::None && type != hitboxes::Type::None &&
		other.layer != hitboxes::Layers::None && layer != hitboxes::Layers::None &&
		(layer == hitboxes::Layers::All || other.layer == hitboxes::Layers::All ||
			layer != other.layer);

	return canIntersect && rect.intersects(other.rect);
}

void ActorHitbox::setPosition(sf::Vector2f pos, bool inv)
{
	auto ogRect = original->getRect();
	if (!inv) rect = sf::FloatRect(pos.x + ogRect.left, pos.y + ogRect.top, ogRect.width, ogRect.height);
	else rect = sf::FloatRect(pos.x + frameWidth - ogRect.left - ogRect.width, pos.y + ogRect.top, ogRect.width, ogRect.height);
}
