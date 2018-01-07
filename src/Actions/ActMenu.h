#pragma once

#include "Action.h"
#include "BitmapText.h"
#include "Game.h"
#include "Menu.h"
#include <string>
#include "StringText.h"
#include "TextUtils.h"
#include "Variable.h"

class ActMenuAppendText : public Action
{
private:
	std::string id;
	size_t idx;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActMenuAppendText(const std::string& id_, size_t idx_,
		const std::string& text_, TextUtils::TextOp textOp_)
		: id(id_), idx(idx_), textFormat(text_), textOp(textOp_) {}

	ActMenuAppendText(const std::string& id_, size_t idx_,
		const std::string& text_, const std::string& query_)
		: id(id_), idx(idx_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(query_);
	}

	ActMenuAppendText(const std::string& id_,
		size_t idx_, const std::string& format_,
		const std::vector<std::string>& bindings_)
		: id(id_), idx(idx_), textFormat(format_),
		bindings(bindings_), textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() { textOp |= TextUtils::TextOp::Trim; }

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				auto str = button->getText();
				TextUtils::appendText(game, textOp, str, textFormat, bindings);
				button->setText(str);
			}
		}
		return true;
	}
};

class ActMenuClick : public Action
{
private:
	std::string id;
	size_t idx;
	bool playSound;

public:
	ActMenuClick(const std::string& id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				button->click(game, playSound);
			}
		}
		return true;
	}
};

class ActMenuClickVisible : public Action
{
private:
	std::string id;
	size_t idx;
	bool playSound;

public:
	ActMenuClickVisible(const std::string& id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getVisibleItem(idx);
			if (button != nullptr)
			{
				button->click(game, playSound);
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
			auto newRange = std::min(range - (unsigned)scrollBar->Size().y, range);
			auto itemCount = menu->getItemCount();
			if (focus == true)
			{
				auto offset = scrollBar->DrawPosition().y - pos.y;
				auto menuIdx = (size_t)std::round((offset * (float)(itemCount - 1)) / (float)newRange);
				auto btn = menu->getItem(menuIdx);
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
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
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
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				auto text = button->getDrawableText();
				if (text != nullptr)
				{
					auto bitmapText = dynamic_cast<BitmapText*>(text);
					if (bitmapText != nullptr)
					{
						auto newFont = game.Resources().getBitmapFont(idFont);
						if (newFont != nullptr)
						{
							bitmapText->setFont(newFont);
						}
						return true;
					}
					auto stringText = dynamic_cast<StringText*>(text);
					if (stringText != nullptr)
					{
						auto newFont = game.Resources().getFont(idFont);
						if (newFont != nullptr)
						{
							stringText->setFont(*newFont);
						}
					}
				}
			}
		}
		return true;
	}
};

class ActMenuSetIndex : public Action
{
private:
	std::string id;
	Variable idxVar;

public:
	ActMenuSetIndex(const std::string& id_, const Variable& idxVar_)
		: id(id_), idxVar(idxVar_) {}

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto idx = (size_t)game.getVarOrPropLong(idxVar);
			menu->setCurrentIdx(idx);
		}
		return true;
	}
};

class ActMenuSetText : public Action
{
private:
	std::string id;
	size_t idx;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActMenuSetText(const std::string& id_, size_t idx_,
		const std::string& text_, TextUtils::TextOp textOp_)
		: id(id_), idx(idx_), textFormat(text_), textOp(textOp_) {}

	ActMenuSetText(const std::string& id_, size_t idx_,
		const std::string& text_, const std::string& query_)
		: id(id_), idx(idx_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(query_);
	}

	ActMenuSetText(const std::string& id_,
		size_t idx_, const std::string& format_,
		const std::vector<std::string>& bindings_)
		: id(id_), idx(idx_), textFormat(format_),
		bindings(bindings_), textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() { textOp |= TextUtils::TextOp::Trim; }

	virtual bool execute(Game& game)
	{
		auto menu = game.Resources().getResource<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				button->setText(TextUtils::getText(game, textOp, textFormat, bindings));
			}
		}
		return true;
	}
};
