#pragma once

#include "Action.h"
#include "Game.h"

class ActLoadingScreenSetProgress : public Action
{
private:
	int progress;

public:
	ActLoadingScreenSetProgress(int progress_) noexcept : progress(progress_) {}

	bool execute(Game& game) override
	{
		auto loadingScreen = game.getLoadingScreen();
		if (loadingScreen != nullptr)
		{
			loadingScreen->setProgress(progress);
			game.drawLoadingScreen();
			if (loadingScreen->isComplete())
			{
				game.Events().addBack(loadingScreen->getAction(str2int16("complete")));
			}
		}
		return true;
	}
};

class ActLoadingScreenClear : public Action
{
public:
	bool execute(Game& game) noexcept override
	{
		game.setLoadingScreen(nullptr);
		return true;
	}
};
