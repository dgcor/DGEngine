#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseGame(Game& game, std::string filePath, const std::string& fileName);
}
