#include "AnimHandler.h"
#include <iostream>

#define DEBUG 1

AnimHandler::AnimHandler(AnimHolder const& holder, animation::ID id) : holder(holder), anim(&holder.get(id)), animID(id)
{
	anim->setSprite(sprite, frame);
	updateHitboxes();
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
		updateHitboxes();
	}
	else if (changed)
	{
		anim->setSprite(sprite, frame, inv);
		updateHitboxes();
	}

	return animation::ID::None;
}

void AnimHandler::updateHitboxes()
{
	hitboxes.clear();
	auto boxes = anim->getHitboxes(frame);
	for (auto box : boxes)
	{
		hitboxes.push_back(std::make_unique<ActorHitbox>(box, sprite.getPosition(), (prevXDir < 0), sprite.getLocalBounds().width));
	}
}

bool AnimHandler::hits(AnimHandler& other)
{
	auto n = hitboxes.size();
	auto m = other.hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		auto box = hitboxes[i].get();
		for (int j = 0; j < m; j++)
		{
			auto otherBox = other.hitboxes[j].get();
			if (box->getType() == hitboxes::Type::Hurt && box->intersect(otherBox))
			{
				// "replay" the same frame to add some impact
				if (reverseLoop) frame++;
				else frame--;
				return true;
			}
		}
	}

	return false;
}

void AnimHandler::setPosition(sf::Vector2f const pos)
{
	sprite.setPosition(pos);

	auto n = hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		hitboxes[i]->setPosition(pos, (prevXDir < 0));
	}
}

void AnimHandler::changeAnim(animation::ID id)
{
	anim = &holder.get(id);
	animID = id;
	frame = 0;
	frameTime = sf::Time::Zero;
	reverseLoop = false;
	anim->setSprite(sprite, 0, (prevXDir < 0));
	updateHitboxes();
}

void AnimHandler::draw(sf::RenderWindow& window) const
{
#if DEBUG
	sf::RectangleShape s;
	auto n = hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		sf::FloatRect rect = hitboxes[i]->getRect();
		s.setPosition(rect.left, rect.top);
		s.setSize(sf::Vector2f(rect.width, rect.height));
		if (hitboxes[i]->getType() == hitboxes::Type::Hit) s.setFillColor(sf::Color::Green);
		else s.setFillColor(sf::Color::Red);
		window.draw(s);
	}
#endif

	window.draw(sprite);
}
