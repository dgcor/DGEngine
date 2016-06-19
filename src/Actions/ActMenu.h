#pragma once

#include "Action.h"
#include "ActButton.h"
#include "Game.h"
#include "Menu.h"
#include <string>

class ActMenuClick : public Action
{
private:
	std::string id;
	size_t idx;

public:
	ActMenuClick(const std::string& id_, size_t idx_) : id(id_), idx(idx_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Menu>(id);
		if (item != nullptr)
		{
			auto button = item->getItem(idx);
			if (button != nullptr)
			{
				button->click(game, true);
			}
		}
		return true;
	}
};

class ActMenuMoveScrollbar : public Action
{
private:
	std::string idMenu;
	std::string idScrollbar;
	std::string idAnchorTo;
	unsigned range;
	bool focus;

public:
	ActMenuMoveScrollbar(const std::string& idMenu_, const std::string& idScrollbar_,
		const std::string& idAnchorTo_, unsigned range_, bool focus_)
		: idMenu(idMenu_), idScrollbar(idScrollbar_), idAnchorTo(idAnchorTo_),
		range(range_), focus(focus_) {}

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(idMenu);
		auto scrollBar = game.Resources().getResource<UIObject>(idScrollbar);
		auto anchorTo = game.Resources().getResource<UIObject>(idAnchorTo);
		if (menu != nullptr && scrollBar != nullptr && anchorTo != nullptr)
		{
			auto pos = anchorTo->DrawPosition();
			auto newRange = std::max(0u, std::min(range - (unsigned)scrollBar->Size().y, range));
			auto itemCount = menu->getItemCount();
			if (focus == true)
			{
				auto offset = scrollBar->DrawPosition().y - pos.y;
				auto menuIdx = std::round((offset * (float)(itemCount - 1)) / (float)newRange);
				auto btn = menu->getItem(menuIdx + 1);
				if (btn != nullptr)
				{
					auto focused = game.Resources().getFocused();
					if (btn != focused)
					{
						game.Resources().setFocused(btn);
						btn->focus(game);
					}
				}
			}
			if (itemCount > 1)
			{
				auto currIdx = (float)menu->getItemPosition(game.Resources().getFocused());
				pos.y += std::round((float)newRange * (float)(currIdx / (itemCount - 1)));
			}
			scrollBar->Position(pos);
		}
		return true;
	}
};

class ActMenuSetBitmapFont : public Action
{
private:
	std::string id;
	size_t idx;
	std::string idFont;

public:
	ActMenuSetBitmapFont(const std::string& id_, size_t idx_, const std::string& idFont_)
		: id(id_), idx(idx_), idFont(idFont_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Menu>(id);
		if (item != nullptr)
		{
			auto button = item->getItem(idx);
			if (button != nullptr)
			{
				auto text = button->getDrawableText();
				if (text != nullptr)
				{
					auto fontText = dynamic_cast<BitmapText*>(text);
					if (fontText != nullptr)
					{
						auto newFont = game.Resources().getBitmapFont(idFont);
						if (newFont != nullptr)
						{
							fontText->setFont(newFont);
						}
					}
				}
			}
		}
		return true;
	}
};

class ActMenuSetColor : public Action
{
private:
	std::string id;
	size_t idx;
	sf::Color color;

public:
	ActMenuSetColor(const std::string& id_, size_t idx_, const sf::Color& color_)
		: id(id_), idx(idx_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Menu>(id);
		if (item != nullptr)
		{
			auto button = item->getItem(idx);
			if (button != nullptr)
			{
				button->setColor(color);
			}
		}
		return true;
	}
};

class ActMenuSetFont : public Action
{
private:
	std::string id;
	size_t idx;
	std::string idFont;

public:
	ActMenuSetFont(const std::string& id_, size_t idx_, const std::string& idFont_)
		: id(id_), idx(idx_), idFont(idFont_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Menu>(id);
		if (item != nullptr)
		{
			auto button = item->getItem(idx);
			if (button != nullptr)
			{
				auto text = button->getDrawableText();
				if (text != nullptr)
				{
					auto fontText = dynamic_cast<StringText*>(text);
					if (fontText != nullptr)
					{
						auto newFont = game.Resources().getFont(idFont);
						if (newFont != nullptr)
						{
							fontText->setFont(*newFont);
						}
					}
				}
			}
		}
		return true;
	}
};

class ActMenuSetText : public Action
{
private:
	std::string id;
	size_t idx;
	std::string text;

public:
	ActMenuSetText(const std::string& id_, size_t idx_, const std::string& text_)
		: id(id_), idx(idx_), text(text_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Menu>(id);
		if (item != nullptr)
		{
			auto button = item->getItem(idx);
			if (button != nullptr)
			{
				button->setText(game.getVariableString(text));
			}
		}
		return true;
	}
};
