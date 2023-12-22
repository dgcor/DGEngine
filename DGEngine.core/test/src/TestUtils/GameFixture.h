#include "Game/Game.h"

class GameFixture : public Game
{
public:
	using Game::Game;

	void updateSize(const sf::Vector2u& size_)
	{
		oldDrawRegionSize = drawRegionSize;
		drawRegionSize = size_;
	}
};
