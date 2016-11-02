#include "FadeInOut.h"
#include "Game.h"

bool FadeInOut::HasFadeEnded(uint8_t alpha)
{
	if (isFadeOut == true)
	{
		return alpha == 255;
	}
	else
	{
		return alpha == 0;
	}
}

void FadeInOut::UpdateFade(sf::Color& color)
{
	if (isFadeOut == true)
	{
		if ((int)color.a + (int)fadeOffset < 255)
		{
			color.a += fadeOffset;
		}
		else
		{
			color.a = 255;
		}
	}
	else
	{
		if ((int)color.a - (int)fadeOffset > 0)
		{
			color.a -= fadeOffset;
		}
		else
		{
			color.a = 0;
		}
	}
}

void FadeInOut::update(Game& game)
{
	if (updateEnableInput == true)
	{
		updateEnableInput = false;
		game.enableInput(enableInput);
	}

	currentTime += game.getElapsedTime();

	if (currentTime >= frameTime)
	{
		currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

		auto color = getFillColor();

		if (HasFadeEnded(color.a) == true)
		{
			if (action != nullptr)
			{
				game.Events().addBack(action);
			}
			if (enableInput == false)
			{
				game.enableInput(true);
			}
			game.setFadeInOut(nullptr);
		}
		else
		{
			UpdateFade(color);
			setFillColor(color);
		}
	}
}
