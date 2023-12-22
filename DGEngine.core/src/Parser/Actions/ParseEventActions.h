#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseEventAdd(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseEventDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseEventDeleteAll();

	std::shared_ptr<Action> parseEventResetTime(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseEventTimeout(const rapidjson::Value& elem);
}
