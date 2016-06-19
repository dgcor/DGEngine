#include "View2.h"
#include "Game.h"

void View2::setViewport(const Game& game)
{
	const auto& windowSize = game.WindowTex().getSize();
	const auto& gameSize = game.WindowSize();
	auto x = position.x;
	if (windowSize.x > gameSize.x)
	{
		x += ((float)windowSize.x / 2.f) - ((float)gameSize.x / 2.f);
	}
	x /= (float)windowSize.x;
	auto y = position.y;
	if (windowSize.y > gameSize.y)
	{
		y += ((float)windowSize.y / 2.f) - ((float)gameSize.y / 2.f);
	}
	y /= (float)windowSize.y;
	auto w = getSize().x / (float)windowSize.x;
	auto h = getSize().y / (float)windowSize.y;

	sf::View::setViewport(sf::FloatRect(x, y, w, h));
}
