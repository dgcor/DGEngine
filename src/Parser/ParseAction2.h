#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2
{
	std::shared_ptr<Action> parseActionElem(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
}
