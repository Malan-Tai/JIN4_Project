#pragma once
#include "ControllableActor.h"

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
	static Lens& instance();
	void init(int w, int h);

	void draw(sf::RenderWindow& window) const;

	void switchCurrentLens(float left, float right);

private:
	LensColors leftLens		= LensColors::Red;
	LensColors curLeftLens	= LensColors::None;
	LensColors rightLens	= LensColors::Blue;
	LensColors curRightLens = LensColors::None;

	sf::RectangleShape fillScreen;
};