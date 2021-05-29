#pragma once
#include "Animation.h"

class AnimHandler
{
public:
	explicit AnimHandler(AnimHolder const& holder, animation::ID id);
	AnimHolder const& getHolder() const;

	int getXDir() const;

	animation::ID update(sf::Time const& elapsed, int xDir);
	void setPosition(sf::Vector2f const pos);
	void changeAnim(animation::ID id);

	void draw(sf::RenderWindow& window) const;

private:
	int frame = 0;
	bool reverseLoop = false;
	sf::Time frameTime = sf::Time::Zero;

	int prevXDir = 1;
	
	Animation const* anim;
	animation::ID animID;
	sf::Sprite sprite;

	AnimHolder const& holder;
};