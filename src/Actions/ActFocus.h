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

class ActFocusSet : public Action
{
private:
	std::string id;
	bool focus;

public:
	ActFocusSet(const std::string& id_, bool focus_) : id(id_), focus(focus_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Button>(id);
		if (item != nullptr)
		{
			if (item != game.Resources().getFocused())
			{
				game.Resources().setFocused(item);
				if (focus == true &&
					item == game.Resources().getFocused())
				{
					item->focus(game);
				}
			}
		}
		return true;
	}
};
