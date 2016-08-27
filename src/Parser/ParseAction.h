#pragma once

#include "Actions/Action.h"
#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Action> parseAction(Game& game, const rapidjson::Value& elem);
}
