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
	float changeAnim(animation::ID id);

	void draw(sf::RenderWindow& window) const;
	void drawBar(sf::RenderWindow& window, float value = 0, float maxValue = 0, sf::Color color = sf::Color::Red) const;
	void drawBar(sf::RenderWindow& window, float value = 0, float fantomValue = 0, float maxValue = 0, sf::Color color = sf::Color::Red, sf::Color fantomColor = sf::Color::Yellow) const;
	void drawBar(sf::RenderWindow& window, float x, float y, float w, float h, float value = 0, float fantomValue = 0, float maxValue = 0, sf::Color color = sf::Color::Red, sf::Color fantomColor = sf::Color::Yellow) const;

	bool takePoiseDamage(float poiseDmg);
	float getPoiseDamage() const;
	float getDamageMultiplier() const;

	float getContinuousStaminaCost() const;

private:
	int frame = 0;
	bool reverseLoop = false;
	sf::Time frameTime = sf::Time::Zero;

	float prevXDir = 1;

	float poiseHP = 0;
	//float const poiseHeal = 10;
	
	Animation const* anim;
	animation::ID animID;
	sf::Sprite sprite;

	float barWidth = 0;

	std::vector<ActorHitbox> hitboxes{};
	std::vector<AnimHandler const*> hitEnemies{};

	AnimHolder const& holder;
};