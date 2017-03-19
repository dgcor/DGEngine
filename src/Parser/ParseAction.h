#pragma once

#include "Actions/Action.h"
#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Action> getActionKey(Game& game, const rapidjson::Value& elem, const char* key);
	std::shared_ptr<Action> parseAction(Game& game, const rapidjson::Value& elem);
	void parseActionAndExecute(Game& game, const rapidjson::Value& elem);
}
