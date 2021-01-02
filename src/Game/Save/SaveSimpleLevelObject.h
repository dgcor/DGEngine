#pragma once

#include "SaveProperties.h"

class Game;
class Level;
class SimpleLevelObject;

namespace Save
{
	void serialize(void* serializeObj, const Properties& props,
		const Game& game, const Level& level, const SimpleLevelObject& obj);
}
