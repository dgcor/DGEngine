#pragma once

#include "Action.h"
#include "Game.h"
#include "InputText.h"

class ActInputTextClick : public Action
{
private:
	std::string id;

public:
	ActInputTextClick(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		auto text = game.Resources().getDrawable<InputText>(id);
		if (text != nullptr)
		{
			text->click(game);
		}
		return true;
	}
};
