#pragma once

#include "Actions/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser
{
	std::shared_ptr<Action> parseActionGameElem(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
}
