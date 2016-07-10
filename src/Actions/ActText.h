#pragma once

#include "Action.h"
#include "Game.h"
#include "Text2.h"

class ActTextSetText : public Action
{
private:
	std::string id;
	std::string text;
	int horizSpaceOffset{ 0 };
	int vertSpaceOffset{ 0 };
	bool hasHorizSpaceOffset{ false };
	bool hasVertSpaceOffset{ false };

public:
	ActTextSetText(const std::string& id_, const std::string& text_)
		: id(id_), text(text_) {}

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
			item->setText(game.getVariableString(text));
		}
		return true;
	}
};
