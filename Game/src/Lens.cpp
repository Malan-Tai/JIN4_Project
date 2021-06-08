#include "Lens.h"

Lens& Lens::instance()
{
	static auto instance = new Lens();
	return *instance;
}

void Lens::init(int w, int h)
{
	fillScreen.setSize(sf::Vector2f(w, h));
	fillScreen.setFillColor(sf::Color::Transparent);
}

void Lens::draw(sf::RenderWindow& window) const
{
	window.draw(fillScreen);
}

void Lens::switchCurrentLens(float left, float right)
{
	if (left != 0)
	{
		if (curLeftLens == leftLens) curLeftLens = LensColors::None;
		else curLeftLens = leftLens;
	}

	if (right != 0)
	{
		if (curRightLens == rightLens) curRightLens = LensColors::None;
		else curRightLens = rightLens;
	}

	sf::Uint8 r = 0;
	sf::Uint8 g = 0;
	sf::Uint8 b = 0;
	sf::Uint8 a = 31;

	if (curLeftLens == LensColors::None && curRightLens == LensColors::None)	a = 0;

	if (curLeftLens == LensColors::Red || curRightLens == LensColors::Red)		r = 255;
	if (curLeftLens == LensColors::Green || curRightLens == LensColors::Green)	g = 255;
	if (curLeftLens == LensColors::Blue || curRightLens == LensColors::Blue)	b = 255;

	fillScreen.setFillColor(sf::Color(r, g, b, a));
}

LensColors Lens::getLeftLens() const
{
	return curLeftLens;
}

LensColors Lens::getRightLens() const
{
	return curRightLens;
}
