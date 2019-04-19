#include "FadeInOut.h"
#include "Game.h"

bool FadeInOut::HasFadeEnded() noexcept
{
	if (isFadeOut == true)
	{
		return color.a == 255;
	}
	else
	{
		return color.a == 0;
	}
}

void FadeInOut::UpdateFade() noexcept
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

void FadeInOut::Reset(sf::Color color_, bool isFadeOut_, bool enableInput_, uint8_t fadeOffset_,
	const sf::Time& timeout_, const std::shared_ptr<Action>& action_)
{
	color = color_;
	if (enableInput != enableInput_)
	{
		updateEnableInput = true;
	}
	isFadeOut = isFadeOut_;
	enableInput = enableInput_;
	fadeOffset = fadeOffset_;
	elapsedTime = timeout_;
	action = action_;
	running = true;
}

void FadeInOut::update(Game& game) noexcept
{
	if (running == false)
	{
		return;
	}
	if (updateEnableInput == true)
	{
		updateEnableInput = false;
		game.EnableInput(enableInput);
	}

	elapsedTime.update(game.getElapsedTime(), [&]()
	{
		if (HasFadeEnded() == true)
		{
			if (action != nullptr)
			{
				game.Events().addBack(action);
			}
			if (enableInput == false)
			{
				game.EnableInput(true);
			}
			action = nullptr;
			running = false;
			color.a = 0;
		}
		else
		{
			UpdateFade();
		}
	});
}
