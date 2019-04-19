#pragma once

#include <string_view>

class Game;

namespace Parser
{
	void parseGame(Game& game, const std::string_view filePath, const std::string_view fileName);
}
