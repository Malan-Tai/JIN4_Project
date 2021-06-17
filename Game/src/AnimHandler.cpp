#include "AnimHandler.h"
#include <iostream>

#define DEBUG 0

AnimHandler::AnimHandler(AnimHolder const& holder, animation::ID id) : holder(holder), anim(&holder.get(id)), animID(id)
{
	anim->setSprite(sprite, frame);
	updateHitboxes();
}

AnimHolder const& AnimHandler::getHolder() const
{
	return holder;
}

// get the direction the sprite is looking
float AnimHandler::getXDir() const
{
	return prevXDir;
}

animation::ID AnimHandler::update(sf::Time const& elapsed, float xDir)
{
	if (poiseHP < anim->poiseHP) poiseHP = std::min((float)anim->poiseHP, poiseHP + poiseHeal * elapsed.asSeconds());

	frameTime += elapsed;
	if (xDir == 0) xDir = prevXDir;		// xDir cannot be 0 anymore
	bool changed = xDir != prevXDir;	// true if the looking direction has changed
	bool inv = xDir < 0;
	prevXDir = xDir;

	if (frameTime.asMilliseconds() >= anim->getTimeForFrame(frame))
	{
		if (!reverseLoop) // cycle through frames forwards
		{
			frame++;
			if (frame == anim->nbFrames) // anim ended
			{
				if (!anim->looping && !anim->reverseLoop) return animID;
				frame = 0;
			}
			else if (anim->reverseLoop && frame == anim->nbFrames - 1) // last frame, needs to reverse
			{
				reverseLoop = true;
			}
		}
		else // cycle through frames backwards
		{
			frame--;
			if (frame == 0) // reverse anim ended
			{
				reverseLoop = false;
			}
		}
		
		frameTime = sf::Time::Zero;
		anim->setSprite(sprite, frame, inv);
		updateHitboxes();
	}
	else if (changed) // changed direction but didn't do this already because of a new frame
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
		hitboxes.emplace_back(box, sprite.getPosition(), (prevXDir < 0), sprite.getLocalBounds().width);
	}
}

// checks whether this animation hits the other one
bool AnimHandler::hits(AnimHandler& other)
{
	auto n = hitboxes.size();
	auto m = other.hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		auto box = hitboxes[i];
		for (int j = 0; j < m; j++)
		{
			auto const& otherBox = other.hitboxes[j];
			if (box.getType() == hitboxes::Type::Hurt && box.intersect(&otherBox))
			{
				for (auto hit : hitEnemies)
				{
					if (hit == &other) return false;
				}

				hitEnemies.push_back(&other);

				// "replay" the same frame to add some impact
				if (reverseLoop) frame++;
				else frame--;
				frameTime = sf::Time::Zero;
				return true;
			}
		}
	}

	return false;
}

// checks whether the animation collides with the level
float AnimHandler::collides(Level const& level, bool isOnGround) const
{
	float maxAbs = 0;
	float res = 0;

	auto n = hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		if (hitboxes[i].getType() == hitboxes::Type::Hit)
		{
			float dy = level.collides(&hitboxes[i], isOnGround);
			float ady = abs(dy);
			if (ady > maxAbs)
			{
				maxAbs = ady;
				res = dy;
			}
		}
	}
	return res;
}

void AnimHandler::setPosition(sf::Vector2f const pos)
{
	sprite.setPosition(pos);

	auto n = hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		hitboxes[i].setPosition(pos, (prevXDir < 0));
	}
}

int AnimHandler::changeAnim(animation::ID id)
{
	if (id == animID) return 0;

	anim = &holder.get(id);
	animID = id;
	frame = 0;
	frameTime = sf::Time::Zero;
	reverseLoop = false;
	anim->setSprite(sprite, 0, (prevXDir < 0));
	updateHitboxes();
	hitEnemies.clear();

	poiseHP = anim->poiseHP;

	if (anim->continuousStamCost) return 0;
	return anim->staminaCost;
}

void AnimHandler::draw(sf::RenderWindow& window, int hp, int maxHP) const
{
#if DEBUG
	sf::RectangleShape s;
	auto n = hitboxes.size();
	for (int i = 0; i < n; i++)
	{
		sf::FloatRect rect = hitboxes[i].getRect();
		s.setPosition(rect.left, rect.top);
		s.setSize(sf::Vector2f(rect.width, rect.height));
		if (hitboxes[i].getType() == hitboxes::Type::Hit) s.setFillColor(sf::Color::Green);
		else s.setFillColor(sf::Color::Red);
		window.draw(s);
	}
#endif

	window.draw(sprite);

	// hp bar
	if (maxHP != 0 && hp < maxHP)
	{
		auto size = sprite.getLocalBounds();
		auto global = sprite.getGlobalBounds();

		float totalWidth = size.width;
		float hpWidth = (hp / (float)maxHP) * totalWidth;

		sf::RectangleShape fullBar;
		fullBar.setSize(sf::Vector2f{ totalWidth, 10 });
		fullBar.setPosition(sf::Vector2f{ global.left, global.top + size.height + 20 });
		fullBar.setFillColor(sf::Color{ 127, 127, 127 });
		window.draw(fullBar);

		sf::RectangleShape hpBar;
		hpBar.setSize(sf::Vector2f{ hpWidth, 10 });
		hpBar.setPosition(sf::Vector2f{ global.left, global.top + size.height + 20 });
		hpBar.setFillColor(sf::Color::Red);
		window.draw(hpBar);
	}
}

// returns true if the anim is cancelled
bool AnimHandler::takePoiseDamage(int poiseDmg)
{
	if (anim->takesPoiseDmg)
	{
		poiseHP -= poiseDmg;
		return poiseHP <= 0;
	}
	return false;
}

int AnimHandler::getPoiseDamage() const
{
	return anim->poiseDamage;
}

float AnimHandler::getContinuousStaminaCost() const
{
	if (anim->continuousStamCost) return anim->staminaCost;
	return 0;
}
