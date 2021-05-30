#pragma once

#include "Action.h"
#include "BindableText.h"
#include "BitmapText.h"
#include "Game.h"
#include "StringText.h"
#include "TextUtils.h"

class ActTextAppendText : public Action
{
private:
	std::string id;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActTextAppendText(const std::string_view id_, const std::string_view text_,
		TextUtils::TextOp textOp_) : id(id_), textFormat(text_), textOp(textOp_) {}

	ActTextAppendText(const std::string_view id_, const std::string_view text_,
		const std::string_view query_) : id(id_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(std::string(query_));
	}

	ActTextAppendText(const std::string_view id_, const std::string_view format_,
		std::vector<std::string>&& bindings_) : id(id_),
		textFormat(format_), bindings(std::move(bindings_)),
		textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() noexcept { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void ReplaceAll() noexcept { textOp |= TextUtils::TextOp::ReplaceAll; }
	void Trim() noexcept { textOp |= TextUtils::TextOp::Trim; }

	bool execute(Game& game) override
	{
		auto text = game.Resources().getDrawable<BindableText>(id);
		if (text != nullptr)
		{
			auto str = text->getText();
			TextUtils::appendText(game, textOp, str, textFormat, bindings);
			text->setText(str);
		}
		return true;
	}
};

class ActTextSetColor : public Action
{
private:
	std::string id;
	sf::Color color;

public:
	ActTextSetColor(const std::string_view id_, const sf::Color& color_) : id(id_), color(color_) {}

	bool execute(Game& game) override
	{
		auto text = game.Resources().getDrawable<Text>(id);
		if (text != nullptr)
		{
			text->setColor(color);
		}
		return true;
	}
};

class ActTextSetFont : public Action
{
private:
	std::string id;
	std::string idFont;

public:
	ActTextSetFont(const std::string_view id_, const std::string_view idFont_)
		: id(id_), idFont(idFont_) {}

	bool execute(Game& game) override
	{
		auto text2 = game.Resources().getDrawable<Text>(id);
		if (text2 != nullptr)
		{
			auto newFont = game.Resources().getFont(idFont);
			auto text = text2->getDrawableText();
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
		return true;
	}
};

class ActTextSetSpacing : public Action
{
private:
	std::string id;
	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	bool hasHorizSpaceOffset{ false };
	bool hasVertSpaceOffset{ false };

public:
	ActTextSetSpacing(const std::string_view id_) : id(id_) {}

	void setHorizontalSpaceOffset(int offset) noexcept
	{
		horizSpaceOffset = offset;
		hasHorizSpaceOffset = true;
	}

	void setVerticalSpaceOffset(int offset) noexcept
	{
		vertSpaceOffset = offset;
		hasVertSpaceOffset = true;
	}

	bool execute(Game& game) override
	{
		auto text = game.Resources().getDrawable<Text>(id);
		if (text != nullptr)
		{
			if (hasHorizSpaceOffset == true)
			{
				text->setHorizontalSpaceOffset(horizSpaceOffset);
			}
			if (hasVertSpaceOffset == true)
			{
				text->setVerticalSpaceOffset(vertSpaceOffset);
			}
		}
		return true;
	}
};

class ActTextSetText : public Action
{
private:
	std::string id;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActTextSetText(const std::string_view id_, const std::string_view text_,
		TextUtils::TextOp textOp_) : id(id_), textFormat(text_), textOp(textOp_) {}

	ActTextSetText(const std::string_view id_, const std::string_view text_,
		const std::string_view query_) : id(id_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(std::string(query_));
	}

	ActTextSetText(const std::string_view id_, const std::string_view format_,
		std::vector<std::string>&& bindings_) : id(id_),
		textFormat(format_), bindings(std::move(bindings_)),
		textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() noexcept { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void ReplaceAll() noexcept { textOp |= TextUtils::TextOp::ReplaceAll; }
	void Trim() noexcept { textOp |= TextUtils::TextOp::Trim; }

	bool execute(Game& game) override
	{
		auto text = game.Resources().getDrawable<BindableText>(id);
		if (text != nullptr)
		{
			text->setText(TextUtils::getText(game, textOp, textFormat, bindings));
		}
		return true;
	}
};
