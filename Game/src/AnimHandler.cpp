#include "AnimHandler.h"

AnimHandler::AnimHandler(AnimHolder const& holder, animation::ID id) : holder(holder), anim(&holder.get(id)), animID(id)
{
	anim->setSprite(sprite, frame);
}

AnimHolder const& AnimHandler::getHolder() const
{
	return holder;
}

int AnimHandler::getXDir() const
{
	return prevXDir;
}

animation::ID AnimHandler::update(sf::Time const& elapsed, int xDir)
{
	frameTime += elapsed;
	if (xDir == 0) xDir = prevXDir; // xDir cannot be 0 anymore
	bool changed = xDir != prevXDir;
	bool inv = xDir < 0;
	prevXDir = xDir;
	if (frameTime.asMilliseconds() >= anim->timePerFrame)
	{
		if (!reverseLoop)
		{
			frame++;
			if (frame == anim->nbFrames)
			{
				if (!anim->looping && !anim->reverseLoop) return animID;
				frame = 0;
			}
			else if (anim->reverseLoop && frame == anim->nbFrames - 1)
			{
				reverseLoop = true;
			}
		}
		else
		{
			frame--;
			if (frame == 0)
			{
				reverseLoop = false;
			}
		}
		
		frameTime = sf::Time::Zero;
		anim->setSprite(sprite, frame, inv);
	}
	else if (changed)
	{
		anim->setSprite(sprite, frame, inv);
	}

	return animation::ID::None;
}

void AnimHandler::setPosition(sf::Vector2f const pos)
{
	sprite.setPosition(pos);
}

void AnimHandler::changeAnim(animation::ID id)
{
	anim = &holder.get(id);
	animID = id;
	frame = 0;
	frameTime = sf::Time::Zero;
	anim->setSprite(sprite, 0, (prevXDir < 0));
}

void AnimHandler::draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}
