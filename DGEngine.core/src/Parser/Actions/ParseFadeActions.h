#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseGameFadeIn(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameFadeOut(Game& game, const rapidjson::Value& elem);
}
