#pragma once

#include "SaveProperties.h"
#include <string_view>

class Game;
class Level;
class LevelObject;

namespace Save
{
	void save(const std::string_view filePath, Properties& props,
		const Game& game, const Level& level);

	void serialize(void* serializeObj, Properties& props,
		const Game& game, const Level& level);
}
