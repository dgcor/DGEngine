#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseLoad(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLoadJson(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLoadRandom(const rapidjson::Value& elem);
}
