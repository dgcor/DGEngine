#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseToggleVisible(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseSetVisible(const rapidjson::Value& elem);
}
