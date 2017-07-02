#pragma once

#include "Action.h"
#include "Game.h"
#include "TextUtils.h"
#include "UIText.h"

class ActUITextAppendText : public Action
{
private:
	std::string id;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActUITextAppendText(const std::string& id_, const std::string& text_,
		TextUtils::TextOp textOp_) : id(id_), textFormat(text_), textOp(textOp_) {}

	ActUITextAppendText(const std::string& id_, const std::string& text_,
		const std::string& query_) : id(id_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(query_);
	}

	ActUITextAppendText(const std::string& id_, const std::string& format_,
		const std::vector<std::string>& bindings_) : id(id_),
		textFormat(format_), bindings(bindings_),
		textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() { textOp |= TextUtils::TextOp::Trim; }

	virtual bool execute(Game& game)
	{
		auto text = game.Resources().getResource<UIText>(id);
		if (text != nullptr)
		{
			text->setText(text->getText() +
				TextUtils::getText(game, textOp, textFormat, bindings));
		}
		return true;
	}
};

class ActUITextSetSpacing : public Action
{
private:
	std::string id;
	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	bool hasHorizSpaceOffset{ false };
	bool hasVertSpaceOffset{ false };

public:
	ActUITextSetSpacing(const std::string& id_) : id(id_) {}

	void setHorizontalSpaceOffset(int offset)
	{
		horizSpaceOffset = offset;
		hasHorizSpaceOffset = true;
	}

	void setVerticalSpaceOffset(int offset)
	{
		vertSpaceOffset = offset;
		hasVertSpaceOffset = true;
	}

	virtual bool execute(Game& game)
	{
		auto text = game.Resources().getResource<UIText>(id);
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

class ActUITextSetText : public Action
{
private:
	std::string id;
	std::string textFormat;
	std::vector<std::string> bindings;
	TextUtils::TextOp textOp;

public:
	ActUITextSetText(const std::string& id_, const std::string& text_,
		TextUtils::TextOp textOp_) : id(id_), textFormat(text_), textOp(textOp_) {}

	ActUITextSetText(const std::string& id_, const std::string& text_,
		const std::string& query_) : id(id_), textFormat(text_),
		textOp(TextUtils::TextOp::Query)
	{
		bindings.push_back(query_);
	}

	ActUITextSetText(const std::string& id_, const std::string& format_,
		const std::vector<std::string>& bindings_) : id(id_),
		textFormat(format_), bindings(bindings_),
		textOp(TextUtils::TextOp::FormatString) {}

	void RemoveEmptyLines() { textOp |= TextUtils::TextOp::RemoveEmptyLines; }
	void Trim() { textOp |= TextUtils::TextOp::Trim; }

	virtual bool execute(Game& game)
	{
		auto text = game.Resources().getResource<UIText>(id);
		if (text != nullptr)
		{
			text->setText(TextUtils::getText(game, textOp, textFormat, bindings));
		}
		return true;
	}
};
