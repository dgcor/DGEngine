#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseResourceAdd(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseResourceBringToFront(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseResourceIgnore(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseResourcePop(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseResourcePopAll(const rapidjson::Value& elem);
}
