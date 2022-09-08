#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseVariableAdd(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseVariableClear(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseVariableSave(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseVariableSet(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseVariableSetIfNull(const rapidjson::Value& elem);
}
