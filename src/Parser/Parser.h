#pragma once

#include "Game.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseGame(Game& game, const std::string_view filePath, const std::string_view fileName);
}
