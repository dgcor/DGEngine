#pragma once

#include "Action.h"
#include "Game.h"
#include "GameUtils.h"
#include "Text2.h"

class ActTextSetColor : public Action
{
private:
	std::string id;
	sf::Color color;

public:
	ActTextSetColor(const std::string& id_, const sf::Color& color_) : id(id_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto text = game.Resources().getResource<Text2>(id);
		if (text != nullptr)
		{
			text->setColor(color);
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
	ActTextSetSpacing(const std::string& id_) : id(id_) {}

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
		auto text = game.Resources().getResource<Text2>(id);
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

	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	bool hasHorizSpaceOffset{ false };
	bool hasVertSpaceOffset{ false };

public:
	ActTextSetText(const std::string& id_, const std::string& text_)
		: id(id_), textFormat(text_) {}

	ActTextSetText(const std::string& id_, const std::string& format_,
		const std::vector<std::string>& bindings_) : id(id_),
		textFormat(format_), bindings(bindings_) {}

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
		auto text = game.Resources().getResource<Text2>(id);
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
			if (bindings.empty() == true)
			{
				text->setText(game.getVariableString(textFormat));
			}
			else
			{
				text->setText(Text2::getFormatString(game, bindings, textFormat));
			}
		}
		return true;
	}
};

class ActTextSetTextFromQuery : public Action
{
private:
	std::string id;
	std::string query;
	std::string newText;
	bool trimText;
	bool removeEmptyLines;

	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	bool hasHorizSpaceOffset{ false };
	bool hasVertSpaceOffset{ false };

public:
	ActTextSetTextFromQuery(const std::string& id_, const std::string& query_,
		const std::string& text_, bool trimText_, bool removeEmptyLines_) : id(id_),
		query(query_), newText(text_), trimText(trimText_), removeEmptyLines(removeEmptyLines_) {}

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
		auto text = game.Resources().getResource<Text2>(id);
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
			auto queryable = game.getQueryable(query);
			std::string str = newText;
			if (queryable != nullptr)
			{
				str = GameUtils::replaceStringWithQueryable(newText, *queryable);
			}
			if (trimText == true)
			{
				str = Utils::trim(str, " \t\r\n");
			}
			if (removeEmptyLines == true)
			{
				str = Utils::removeEmptyLines(str);
			}
			text->setText(str);
		}
		return true;
	}
};
