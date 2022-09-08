#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseAnimationPause(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAnimationSetAnimation(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAnimationSetRefresh(const rapidjson::Value& elem);
}
