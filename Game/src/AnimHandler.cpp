#include "AnimHandler.h"

AnimHandler::AnimHandler(AnimHolder const& holder, animation::ID id) : holder(holder), anim(&holder.get(id))
{
	anim->setSprite(sprite, frame);
}

AnimHolder const& AnimHandler::getHolder() const
{
	return holder;
}

void AnimHandler::update(sf::Time const& elapsed, int xDir)
{
	frameTime += elapsed;
	if (xDir == 0) xDir = prevXDir; // xDir cannot be 0 anymore
	bool changed = xDir != prevXDir;
	bool inv = xDir < 0;
	if (frameTime.asMilliseconds() >= anim->timePerFrame)
	{
		frame++;
		if (frame == anim->nbFrames) frame = 0;
		frameTime = sf::Time::Zero;

		anim->setSprite(sprite, frame, inv);
	}
	else if (changed)
	{
		anim->setSprite(sprite, frame, inv);
	}
	prevXDir = xDir;
}

void AnimHandler::setPosition(sf::Vector2f const pos)
{
	sprite.setPosition(pos);
}

void AnimHandler::changeAnim(animation::ID id)
{
	anim = &holder.get(id);
	frame = 0;
	frameTime = sf::Time::Zero;
	anim->setSprite(sprite, 0, (prevXDir < 0));
}

void AnimHandler::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}
