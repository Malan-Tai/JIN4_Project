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
	auto l = root.attribute("looping");
	if (!l.empty()) looping = l.as_bool();
	auto rl = root.attribute("reverseLoop");
	if (!rl.empty()) reverseLoop = rl.as_bool();
	auto tpf = root.attribute("timePerFrame");
	if (!tpf.empty()) timePerFrame = tpf.as_float();

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

#if TESTS
int Animation::hitboxesLength() const
{
	return hitboxes.size();
}
#endif
