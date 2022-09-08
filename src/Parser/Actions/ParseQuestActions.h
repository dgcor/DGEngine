#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2::Actions
{
	std::shared_ptr<Action> parseQuestAdd(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseQuestDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseQuestSetState(const rapidjson::Value& elem);
}
