#pragma once

#include "Action.h"
#include "Game.h"
#include "Text2.h"

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
		auto item = game.Resources().getResource<Text2>(id);
		if (item != nullptr)
		{
			if (hasHorizSpaceOffset == true)
			{
				item->setHorizontalSpaceOffset(horizSpaceOffset);
			}
			if (hasVertSpaceOffset == true)
			{
				item->setVerticalSpaceOffset(vertSpaceOffset);
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
		auto item = game.Resources().getResource<Text2>(id);
		if (item != nullptr)
		{
			if (hasHorizSpaceOffset == true)
			{
				item->setHorizontalSpaceOffset(horizSpaceOffset);
			}
			if (hasVertSpaceOffset == true)
			{
				item->setVerticalSpaceOffset(vertSpaceOffset);
			}
			if (bindings.empty() == true)
			{
				item->setText(game.getVariableString(textFormat));
			}
			else
			{
				item->setText(Text2::getFormatString(game, bindings, textFormat));
			}
		}
		return true;
	}
};
