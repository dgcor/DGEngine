#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<CelFile> parseCelFileObj(Game& game, const rapidjson::Value& elem);
	void parseCelFile(Game& game, const rapidjson::Value& elem);
}
