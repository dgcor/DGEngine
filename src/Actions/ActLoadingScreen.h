#pragma once

#include "Action.h"
#include "Game.h"

class ActLoadingScreenSetProgress : public Action
{
private:
	int progress;

public:
	ActLoadingScreenSetProgress(int progress_) noexcept : progress(progress_) {}

	virtual bool execute(Game& game)
	{
		auto loadingScreen = game.getLoadingScreen();
		if (loadingScreen != nullptr)
		{
			loadingScreen->setProgress(progress);
			game.drawLoadingScreen();
			if (loadingScreen->isComplete())
			{
				game.Events().addBack(loadingScreen->getAction());
			}
		}
		return true;
	}
};

class ActLoadingScreenClear : public Action
{
public:
	virtual bool execute(Game& game) noexcept
	{
		game.setLoadingScreen(nullptr);
		return true;
	}
};
