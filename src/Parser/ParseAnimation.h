#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Animation> parseAnimationObj(Game& game, const rapidjson::Value& elem);
	void parseAnimation(Game& game, const rapidjson::Value& elem);
}
