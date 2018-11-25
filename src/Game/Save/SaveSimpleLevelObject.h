#pragma once

#include "SaveProperties.h"

class Game;
class Level;
class SimpleLevelObject;

namespace Save
{
	void serialize(void* serializeObj, Properties& props,
		const Game& game, const Level& level, const SimpleLevelObject& obj);
}
