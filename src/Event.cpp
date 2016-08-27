#include "Event.h"
#include "Game.h"

bool Event::update(Game& game)
{
	if (action == nullptr)
	{
		return true;
	}

	currentTime += game.getElapsedTime();

	if (currentTime >= frameTime)
	{
		auto ret = action->execute(game);

		if (ret == false && frameTime != sf::Time::Zero)
		{
			// reset time, but keep the remainder
			currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());
		}
		else
		{
			currentTime = sf::Time::Zero;
		}
		return ret;
	}
	return false;
}
