#pragma once

#include "SaveProperties.h"
#include <string_view>

class Game;
class Level;

namespace Save
{
	void save(const std::string_view filePath, const Properties& props,
		const Game& game, const Level& level);

	void serialize(void* serializeObj, const Properties& props,
		const Game& game, const Level& level);
}
