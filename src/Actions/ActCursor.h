#pragma once

#include "Action.h"
#include "Game.h"
#include <string>

class ActCursorCenterOnDrawable : public Action
{
private:
	std::string id;

public:
	ActCursorCenterOnDrawable(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto obj = game.Resources().getDrawable(id);
		if (obj != nullptr)
		{
			auto centerPos = obj->Position();
			auto centerSize = obj->Size();
			sf::Vector2i newPos(
				(int)std::round(centerPos.x + (centerSize.x / 2.f)),
				(int)std::round(centerPos.y + (centerSize.y / 2.f))
			);
			game.setMousePosition(newPos);
		}
		return true;
	}
};

class ActCursorEnableOutline : public Action
{
private:
	bool enable;

public:
	ActCursorEnableOutline(bool enable_) noexcept : enable(enable_) {}

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
	ActCursorPop(bool popAll_) noexcept : popAll(popAll_) {}

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
	ActCursorSetColor(const sf::Color& color_) noexcept : color(color_) {}

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
	ActCursorSetOutline(const sf::Color& outline_, const sf::Color& ignore_) noexcept
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

class ActCursorSetPosition : public Action
{
private:
	sf::Vector2i pos;

public:
	ActCursorSetPosition(const sf::Vector2i& pos_) : pos(pos_) {}

	virtual bool execute(Game& game)
	{
		game.setMousePosition(pos);
		return true;
	}
};

class ActCursorShow : public Action
{
private:
	bool show;

public:
	ActCursorShow(bool show_) noexcept : show(show_) {}

	virtual bool execute(Game& game)
	{
		game.setMouseCursorVisible(show);
		return true;
	}
};
