#pragma once

#include "Action.h"
#include "Game.h"
#include <string>

class ActCursorEnableOutline : public Action
{
private:
	bool enable;

public:
	ActCursorEnableOutline(bool enable_) : enable(enable_) {}

	virtual bool execute(Game& game)
	{
		auto cursor = game.Resources().getCursor();
		if (cursor != nullptr)
		{
			cursor->setOutlineEnabled(enable);
		}
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

class ActCursorSetColor : public Action
{
private:
	sf::Color color;

public:
	ActCursorSetColor(const sf::Color& color_) : color(color_) {}

	virtual bool execute(Game& game)
	{
		auto cursor = game.Resources().getCursor();
		if (cursor != nullptr)
		{
			cursor->setColor(color);
		}
		return true;
	}
};

class ActCursorSetOutline : public Action
{
private:
	sf::Color outline;
	sf::Color ignore;

public:
	ActCursorSetOutline(const sf::Color& outline_, const sf::Color& ignore_)
		: outline(outline_), ignore(ignore_) {}

	virtual bool execute(Game& game)
	{
		auto cursor = game.Resources().getCursor();
		if (cursor != nullptr)
		{
			cursor->setOutline(outline, ignore);
		}
		return true;
	}
};

class ActCursorSetPalette : public Action
{
private:
	std::string idPalette;
	sf::Color color;

public:
	ActCursorSetPalette(const std::string& idPalette_, const sf::Color& color_)
		: idPalette(idPalette_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto cursor = game.Resources().getCursor();
		if (cursor != nullptr)
		{
			if (cursor->hasPalette() == true)
			{
				auto palette = game.Resources().getPalette(idPalette);
				if (palette != nullptr)
				{
					cursor->setPalette(palette);
				}
			}
			else
			{
				cursor->setColor(color);
			}
		}
		return true;
	}
};

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
