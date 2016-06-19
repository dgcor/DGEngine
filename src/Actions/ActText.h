#pragma once

#include "Action.h"
#include "Game.h"
#include "Text2.h"

class ActTextSetText : public Action
{
private:
	std::string id;
	std::string text;
	unsigned lineSpacing;

public:
	ActTextSetText(const std::string& id_, const std::string& text_, unsigned lineSpacing_)
		: id(id_), text(text_), lineSpacing(lineSpacing_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Text2>(id);
		if (item != nullptr)
		{
			if (lineSpacing > 0)
			{
				item->setLineSpacing(lineSpacing);
			}
			item->setText(game.getVariableString(text));
		}
		return true;
	}
};
