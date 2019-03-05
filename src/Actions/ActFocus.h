#pragma once

#include "Action.h"
#include "Button.h"
#include "Game.h"

class ActFocusAdd : public Action
{
private:
	std::string id;
	bool setFocus;

public:
	ActFocusAdd(const std::string& id_, bool setFocus_)
		: id(id_), setFocus(setFocus_) {}

	virtual bool execute(Game& game)
	{
		auto button = game.Resources().getDrawableSharedPtr<Button>(id);
		if (button != nullptr)
		{
			game.Resources().addFocused(button);
			if (setFocus == true)
			{
				game.Resources().setFocused(button.get());
			}
		}
		return true;
	}
};

class ActFocusClick : public Action
{
private:
	bool playSound;

public:
	ActFocusClick(bool playSound_) noexcept : playSound(playSound_) {}

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
		auto button = game.Resources().getDrawable<Button>(id);
		if (button != nullptr)
		{
			if (button != game.Resources().getFocused())
			{
				game.Resources().setFocused(button);
				if (focus == true &&
					button == game.Resources().getFocused())
				{
					button->focus(game);
				}
			}
		}
		return true;
	}
};

class ActFocusUpdate : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Resources().updateFocus(game);
		return true;
	}
};
