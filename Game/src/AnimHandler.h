#pragma once
#include "Animation.h"
#include "Level.h"

// class handling animation : updates its frames, its hitboxes, etc
class AnimHandler
{
public:
	explicit AnimHandler(AnimHolder const& holder, animation::ID id);
	AnimHolder const& getHolder() const;

	float getXDir() const;

	animation::ID update(sf::Time const& elapsed, float xDir);
	void updateHitboxes();
	bool hits(AnimHandler& other);
	float collides(Level const& level, bool isOnGround = false) const;

	void setPosition(sf::Vector2f const pos);
	int changeAnim(animation::ID id);

	void draw(sf::RenderWindow& window, int hp = 0, int maxHP = 0) const;

	bool takePoiseDamage(int poiseDmg);
	int getPoiseDamage() const;

	float getContinuousStaminaCost() const;

private:
	int frame = 0;
	bool reverseLoop = false;
	sf::Time frameTime = sf::Time::Zero;

	float prevXDir = 1;

	float poiseHP = 0;
	float const poiseHeal = 10;
	
	Animation const* anim;
	animation::ID animID;
	sf::Sprite sprite;

	std::vector<ActorHitbox> hitboxes{};
	std::vector<AnimHandler const*> hitEnemies{};

	AnimHolder const& holder;
};