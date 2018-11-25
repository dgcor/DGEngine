#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Action> getIfCondition(uint16_t conditionHash16,
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> getInListCondition(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> getMultiIfCondition(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> getSwitchCondition(Game& game, const rapidjson::Value& elem);
}
