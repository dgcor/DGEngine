#include "Event.h"
#include "Game.h"

bool Event::execute(Game& game)
{
	if (action == nullptr)
	{
		return true;
	}

	currentTime += game.getElapsedTime();

	if (currentTime >= timeout)
	{
		auto ret = action->execute(game);

		if (ret == false && timeout != sf::Time::Zero)
		{
			// reset time, but keep the remainder
			currentTime = sf::microseconds(currentTime.asMicroseconds() % timeout.asMicroseconds());
		}
		else
		{
			currentTime = sf::Time::Zero;
		}
		return ret;
	}
	return false;
}
