#pragma once

#include <cstdint>
#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseIfCondition(uint16_t conditionHash16,
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseInListCondition(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMultiIfCondition(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseSwitchCondition(Game& game, const rapidjson::Value& elem);
}
