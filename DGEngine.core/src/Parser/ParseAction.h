#pragma once

#include "Actions/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser
{
	std::shared_ptr<Action> getActionKey(Game& game, const rapidjson::Value& elem, const char* key);

	std::shared_ptr<Action> getActionVal(Game& game, const rapidjson::Value& elem);

	void parseActionAndExecute(Game& game, const rapidjson::Value& elem);
}
