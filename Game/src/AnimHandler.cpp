#include "AnimHandler.h"

AnimHandler::AnimHandler(AnimHolder const& holder, animation::ID id) : holder(holder), anim(holder.get(id))
{
	anim.setSprite(sprite, frame);
}

AnimHolder const& AnimHandler::getHolder() const
{
	return holder;
}

void AnimHandler::update(sf::Time const& elapsed)
{
	frameTime += elapsed;
	if (frameTime.asMilliseconds() >= anim.timePerFrame)
	{
		frame++;
		if (frame == anim.nbFrames) frame = 0;
		frameTime = sf::Time::Zero;

		anim.setSprite(sprite, frame);
	}
}

void AnimHandler::setPosition(sf::Vector2f const pos)
{
	sprite.setPosition(pos);
}

void AnimHandler::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}
