#pragma once

#include "Action.h"
#include "Game.h"
#include "Parser/Parser.h"
#include <string>

class ActEventDelete : public Action
{
private:
	std::string id;

public:
	ActEventDelete(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		game.Events().remove(id);
		return true;
	}
};
