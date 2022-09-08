#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseRandom(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseRandomList(Game& game, const rapidjson::Value& elem);
}
