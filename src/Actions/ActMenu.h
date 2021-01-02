#pragma once

#include "Action.h"
#include "BitmapText.h"
#include "Game.h"
#include "Menu.h"
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
	ActMenuAppendText(const std::string_view id_, size_t idx_,
		const std::string_view text_, TextUtils::TextOp textOp_)
		: id(id_), idx(idx_), textFormat(text_), textOp(textOp_) {}

	ActMenuAppendText(const std::string_view id_, size_t idx_,
		const std::string_view text_, const std::string_view query_)
		: id(id_), idx(idx_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(std::string(query_));
	}

	ActMenuAppendText(const std::string_view id_,
		size_t idx_, const std::string_view format_,
		std::vector<std::string>&& bindings_)
		: id(id_), idx(idx_), textFormat(format_),
		bindings(std::move(bindings_)), textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() noexcept { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() noexcept { textOp |= TextUtils::TextOp::Trim; }

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
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
	ActMenuClick(const std::string_view id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
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
	ActMenuClickVisible(const std::string_view id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
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
	ActMenuMoveScrollbar(const std::string_view idMenu_, const std::string_view idScrollbar_,
		const std::string_view idAnchorTo_, unsigned range_, bool focus_)
		: idMenu(idMenu_), idScrollbar(idScrollbar_), idAnchorTo(idAnchorTo_),
		range(range_), focus(focus_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(idMenu);
		auto scrollBar = game.Resources().getDrawable(idScrollbar);
		auto anchorTo = game.Resources().getDrawable(idAnchorTo);
		if (menu != nullptr && scrollBar != nullptr && anchorTo != nullptr)
		{
			auto pos = anchorTo->DrawPosition();
			auto newRange = std::min(range - (unsigned)scrollBar->Size().y, range);
			auto itemCount = menu->getItemCount();
			if (focus == true && menu->isFocusEnabled() == true)
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

class ActMenuRightClick : public Action
{
private:
	std::string id;
	size_t idx;
	bool playSound;

public:
	ActMenuRightClick(const std::string_view id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				button->rightClick(game, playSound);
			}
		}
		return true;
	}
};

class ActMenuRightClickVisible : public Action
{
private:
	std::string id;
	size_t idx;
	bool playSound;

public:
	ActMenuRightClickVisible(const std::string_view id_, size_t idx_, bool playSound_)
		: id(id_), idx(idx_), playSound(playSound_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getVisibleItem(idx);
			if (button != nullptr)
			{
				button->rightClick(game, playSound);
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
	ActMenuSetColor(const std::string_view id_, size_t idx_, const sf::Color& color_)
		: id(id_), idx(idx_), color(color_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
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
	ActMenuSetFont(const std::string_view id_, size_t idx_, const std::string_view idFont_)
		: id(id_), idx(idx_), idFont(idFont_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
		if (menu != nullptr)
		{
			auto button = menu->getItem(idx);
			if (button != nullptr)
			{
				auto newFont = game.Resources().getFont(idFont);
				auto text = button->getDrawableText();
				if (holdsNullFont(newFont) == false &&
					text != nullptr)
				{
					auto bitmapText = dynamic_cast<BitmapText*>(text);
					if (bitmapText != nullptr &&
						holdsBitmapFont(newFont) == true)
					{
						bitmapText->setFont(std::get<std::shared_ptr<BitmapFont>>(newFont));
						return true;
					}
					auto stringText = dynamic_cast<StringText*>(text);
					if (stringText != nullptr &&
						holdsFreeTypeFont(newFont) == true)
					{
						stringText->setFont(std::get<std::shared_ptr<FreeTypeFont>>(newFont));
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
	bool focus;

public:
	ActMenuSetIndex(const std::string_view id_, const Variable& idxVar_, bool focus_)
		: id(id_), idxVar(idxVar_), focus(focus_) {}

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
		if (menu != nullptr)
		{
			auto idx = (size_t)game.getVarOrPropInt64V(idxVar);
			menu->setCurrentIdx(idx);
			if (focus == true && menu->isFocusEnabled() == true)
			{
				auto btn = menu->getItem();
				auto focused = game.Resources().getFocused();
				if (btn != focused)
				{
					game.Resources().setFocused(btn);
					btn->focus(game);
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
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActMenuSetText(const std::string_view id_, size_t idx_,
		const std::string_view text_, TextUtils::TextOp textOp_)
		: id(id_), idx(idx_), textFormat(text_), textOp(textOp_) {}

	ActMenuSetText(const std::string_view id_, size_t idx_,
		const std::string_view text_, const std::string_view query_)
		: id(id_), idx(idx_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(std::string(query_));
	}

	ActMenuSetText(const std::string_view id_,
		size_t idx_, const std::string_view format_,
		std::vector<std::string>&& bindings_)
		: id(id_), idx(idx_), textFormat(format_),
		bindings(std::move(bindings_)), textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() noexcept { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() noexcept { textOp |= TextUtils::TextOp::Trim; }

	bool execute(Game& game) override
	{
		auto menu = game.Resources().getDrawable<Menu>(id);
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
