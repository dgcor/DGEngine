#include "Event.h"
#include "Game.h"

bool Event::execute(Game& game)
{
	if (action == nullptr)
	{
		return true;
	}
	if (currentTime == sf::Time::Zero)
	{
		// prevents executing events created while loading big files immediately
		currentTime = sf::microseconds(1);
		return false;
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
