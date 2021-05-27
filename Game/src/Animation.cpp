#include "Animation.h"

bool Animation::loadFromFile(std::string const& basename)
{
	auto xml = basename + ".xml";
	auto png = basename + ".png";

	if (!spritesheet.loadFromFile(png)) return false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xml.c_str());
	if (!result) return false;
	
	auto root = doc.child("root");
	auto rootChildren = root.children();
	nbFrames = std::distance(rootChildren.begin(), rootChildren.end());

	auto size = spritesheet.getSize();
	frameWidth = size.x / nbFrames;
	height = size.y;
	
	for (int i = 0; i < nbFrames; i++)
	{
		std::string f = "frame_" + i;
		auto frame = root.child(f.c_str());

		auto frameChildren = frame.children();
		int n = std::distance(frameChildren.begin(), frameChildren.end());

		hitboxes.push_back({});
		hurtboxes.push_back({});

		for (int j = 0; j < n; j++)
		{
			std::string r = "Rect_" + j;
			auto rect = root.child(r.c_str());

			std::string type = rect.attribute("type").value();
			float x = std::stof(rect.attribute("x").value());
			float y = std::stof(rect.attribute("y").value());
			float w = std::stof(rect.attribute("w").value());
			float h = std::stof(rect.attribute("h").value());

			sf::FloatRect box{ x, y, w, h };

			if (type == "hurt") hurtboxes[i].push_back(box);
			else if (type == "hit") hitboxes[i].push_back(box);
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
