#include "Animation.h"
#include <iostream>

bool Animation::loadFromFile(std::string const& basename)
{
	auto xml = basename + "_boxes.xml";
	auto png = basename + ".png";

	if (!spritesheet.loadFromFile(png)) return false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xml.c_str());
	if (!result) return false;
	
	auto root = doc.child("root");

	// anim loops
	auto attr_looping = root.attribute("looping");
	if (!attr_looping.empty()) looping = attr_looping.as_bool();

	auto attr_reverseLoop = root.attribute("reverseLoop");
	if (!attr_reverseLoop.empty()) reverseLoop = attr_reverseLoop.as_bool();

	// poise
	auto attr_takesPoiseDmg = root.attribute("takesPoiseDmg");
	if (!attr_takesPoiseDmg.empty()) takesPoiseDmg = attr_takesPoiseDmg.as_bool();

	auto attr_poiseHP = root.attribute("poiseHP");
	if (!attr_poiseHP.empty()) poiseHP = attr_poiseHP.as_float();

	auto attr_poiseDmg = root.attribute("poiseDmg");
	if (!attr_poiseDmg.empty()) poiseDamage = attr_poiseDmg.as_float();

	// stamina
	auto attr_continuousStam = root.attribute("continuousStamina");
	if (!attr_continuousStam.empty()) continuousStamCost = attr_continuousStam.as_bool();

	auto attr_stamina = root.attribute("stamina");
	if (!attr_stamina.empty()) staminaCost = attr_stamina.as_float();

	// base time per frame (if a specific frame's duration is not given)
	int baseTPF = 500;
	auto attr_timePerFrame = root.attribute("timePerFrame");
	if (!attr_timePerFrame.empty()) baseTPF = attr_timePerFrame.as_int();

	hitboxes::Layers layer = hitboxes::strToLayer(root.attribute("layer").value());

	auto rootChildren = root.children();
	nbFrames = std::distance(rootChildren.begin(), rootChildren.end());

	auto size = spritesheet.getSize();
	frameWidth = size.x / nbFrames;
	height = size.y;
	
	for (int i = 0; i < nbFrames; i++)
	{
		std::string f = "frame_" + std::to_string(i);
		auto frame = root.child(f.c_str());

		int time = baseTPF;
		auto attr_time = frame.attribute("time");
		if (!attr_time.empty()) time = attr_time.as_int();
		timePerFrames.push_back(time);

		float dmg = 1;
		auto attr_dmg = frame.attribute("dmg");
		if (!attr_dmg.empty()) dmg = attr_dmg.as_float();
		damageMultipliers.push_back(dmg);

		auto frameChildren = frame.children();
		int n = std::distance(frameChildren.begin(), frameChildren.end());

		hitboxes.push_back({});

		for (int j = 0; j < n; j++)
		{
			std::string r = "Rect_" + std::to_string(j);
			auto rect = frame.child(r.c_str());

			hitboxes::Type type = hitboxes::strToType(rect.attribute("type").value());
			float x = rect.attribute("x").as_float();
			float y = rect.attribute("y").as_float();
			float w = rect.attribute("width").as_float();
			float h = rect.attribute("height").as_float();

			sf::FloatRect box{ x, y, w, h };

			hitboxes[i].push_back(new Hitbox{ box, layer, type });
		}
	}

	return true;
}

// sets the referenced sprite to the correct frame of the spritesheet,
// taking into account the direction the actor is looking
void Animation::setSprite(sf::Sprite& sprite, int frame, bool inv) const
{
	int x = frame * frameWidth;
	int w = frameWidth;

	if (inv)
	{
		x += frameWidth;
		w *= -1;
	}

	sprite.setTexture(spritesheet);
	sprite.setTextureRect(sf::IntRect(x, 0, w, height));
}

std::vector<Hitbox const*> Animation::getHitboxes(int frame) const
{
	std::vector<Hitbox const*> res{};
	auto n = hitboxes[frame].size();
	for (int i = 0; i < n; i++)
	{
		res.push_back(hitboxes[frame][i]);
	}
	return res;
}

int Animation::getTimeForFrame(int frame) const
{
	return timePerFrames[frame];
}

float Animation::getDamageMultiplier(int frame) const
{
	return damageMultipliers[frame];
}

#if TESTS
int Animation::hitboxesLength() const
{
	return hitboxes.size();
}
#endif
