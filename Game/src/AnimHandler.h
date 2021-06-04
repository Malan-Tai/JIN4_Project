#pragma once
#include "Animation.h"
#include "Level.h"

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
	void changeAnim(animation::ID id);

	void draw(sf::RenderWindow& window) const;

private:
	int frame = 0;
	bool reverseLoop = false;
	sf::Time frameTime = sf::Time::Zero;

	float prevXDir = 1;
	
	Animation const* anim;
	animation::ID animID;
	sf::Sprite sprite;

	std::vector<std::unique_ptr<ActorHitbox>> hitboxes{};
	std::vector<AnimHandler const*> hitEnemies{};

	AnimHolder const& holder;
};