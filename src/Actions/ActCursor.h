#pragma once

#include "Action.h"
#include "Game.h"
#include <string>

class ActCursorShow : public Action
{
private:
	bool show;

public:
	ActCursorShow(bool show_) : show(show_) {}

	virtual bool execute(Game& game)
	{
		game.Window().setMouseCursorVisible(show);
		return true;
	}
};

class ActCursorPop : public Action
{
private:
	bool popAll;

public:
	ActCursorPop(bool popAll_) : popAll(popAll_) {}

	virtual bool execute(Game& game)
	{
		game.Resources().popCursor(popAll);
		game.updateCursorPosition();
		return true;
	}
};
