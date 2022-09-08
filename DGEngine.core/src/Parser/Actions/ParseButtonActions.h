#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseButtonClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseButtonEnable(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseButtonRightClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseButtonSetColor(const rapidjson::Value& elem);
}
