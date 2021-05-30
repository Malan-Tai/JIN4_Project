#include "Hitbox.h"

Hitbox::Hitbox(sf::FloatRect rect, hitboxes::Layers layer, hitboxes::Type type) : rect(rect), layer(layer), type(type)
{
}

hitboxes::Type Hitbox::getType() const
{
	return type;
}

sf::FloatRect Hitbox::getRect() const
{
	return rect;
}

hitboxes::Layers Hitbox::getLayer() const
{
	return layer;
}

hitboxes::Type hitboxes::strToType(std::string const& str)
{
	if (str == "hit") return hitboxes::Type::Hit;
	else if (str == "hurt") return hitboxes::Type::Hurt;
	else return hitboxes::Type::None;
}

hitboxes::Layers hitboxes::strToLayer(std::string const& str)
{
	if (str == "all") return hitboxes::Layers::All;
	else if (str == "ally") return hitboxes::Layers::Ally;
	else if (str == "enemy") return hitboxes::Layers::Enemy;
	else return hitboxes::Layers::None;
}
