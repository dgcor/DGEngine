#pragma once

#include "Action.h"
#include "Button.h"
#include "Game.h"

class ActFocusAdd : public Action
{
private:
	std::string id;
	std::string resource;
	bool setFocus;

public:
	ActFocusAdd(const std::string_view id_, const std::string_view resource_, bool setFocus_)
		: id(id_), resource(resource_), setFocus(setFocus_) {}

	bool execute(Game& game) override
	{
		auto button = game.Resources().getDrawableSharedPtr<Button>(id);
		if (button != nullptr)
		{
			game.Resources().addFocused(button, resource);
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

	bool execute(Game& game) override
	{
		game.Resources().clickFocused(game, playSound);
		return true;
	}
};

class ActFocusMoveDown : public Action
{
public:
	bool execute(Game& game) override
	{
		game.Resources().moveFocusDown(game);
		return true;
	}
};

class ActFocusMoveUp : public Action
{
public:
	bool execute(Game& game) override
	{
		game.Resources().moveFocusUp(game);
		return true;
	}
};

class ActFocusRightClick : public Action
{
private:
	bool playSound;

public:
	ActFocusRightClick(bool playSound_) noexcept : playSound(playSound_) {}

	bool execute(Game& game) override
	{
		game.Resources().rightClickFocused(game, playSound);
		return true;
	}
};

class ActFocusSet : public Action
{
private:
	std::string id;
	bool focus;

public:
	ActFocusSet(const std::string_view id_, bool focus_) : id(id_), focus(focus_) {}

	bool execute(Game& game) override
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
	bool execute(Game& game) override
	{
		game.Resources().updateFocus(game);
		return true;
	}
};
