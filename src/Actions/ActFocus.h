#pragma once

#include "Action.h"
#include "Button.h"
#include "Game.h"

class ActFocusClick : public Action
{
private:
	bool playSound;

public:
	ActFocusClick(bool playSound_) : playSound(playSound_) {}

	virtual bool execute(Game& game)
	{
		game.Resources().clickFocused(game, playSound);
		return true;
	}
};

class ActFocusMoveDown : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Resources().moveFocusDown(game);
		return true;
	}
};

class ActFocusMoveUp : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Resources().moveFocusUp(game);
		return true;
	}
};
