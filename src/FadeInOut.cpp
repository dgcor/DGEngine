#include "FadeInOut.h"
#include "Game.h"

bool FadeInOut::HasFadeEnded(uint8_t alpha) noexcept
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

void FadeInOut::UpdateFade(sf::Color& color) noexcept
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

void FadeInOut::update(Game& game) noexcept
{
	if (updateEnableInput == true)
	{
		updateEnableInput = false;
		game.EnableInput(enableInput);
	}

	currentTime += game.getElapsedTime();

	while (currentTime >= frameTime)
	{
		currentTime -= frameTime;

		auto color = getFillColor();

		if (HasFadeEnded(color.a) == true)
		{
			if (action != nullptr)
			{
				game.Events().addBack(action);
			}
			if (enableInput == false)
			{
				game.EnableInput(true);
			}
			game.setFadeInOut(nullptr);
			return;
		}
		else
		{
			UpdateFade(color);
			setFillColor(color);
		}
	}
}
