#pragma once
#include <SFML/Graphics.hpp>

enum class LensColors
{
	None,
	Red,
	Green,
	Blue
};

class Lens
{
public:
	explicit Lens(int w, int h);

	void draw(sf::RenderWindow& window) const;

	void switchCurrentLens(float left, float right);

	LensColors getLeftLens() const;
	LensColors getRightLens() const;

private:
	LensColors leftLens		= LensColors::Red;
	LensColors curLeftLens	= LensColors::None;
	LensColors rightLens	= LensColors::Blue;
	LensColors curRightLens = LensColors::None;

	sf::RectangleShape fillScreen;
};