#include "Event.h"
#include "Game.h"

bool Event::execute(Game& game)
{
	if (action == nullptr)
	{
		return true;
	}
	if (elapsedTime.currentTime == sf::Time::Zero)
	{
		// prevents executing events created while loading big files immediately
		elapsedTime.currentTime = sf::microseconds(1);
		return false;
	}
	if (elapsedTime.timeout == sf::Time::Zero ||
		elapsedTime.update(game.getElapsedTime()) == true)
	{
		auto ret = action->execute(game);
		if (ret == true || elapsedTime.timeout == sf::Time::Zero)
		{
			resetTime();
		}
		return ret;
	}
	return false;
}
