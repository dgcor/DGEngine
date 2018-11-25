#pragma once

#include "SaveProperties.h"

class Game;
class Level;
class Item;

namespace Save
{
	// if customProp is not null, it points to the inventory index of the item (size_t)
	void serialize(void* serializeObj, Properties& props,
		const Game& game, const Level& level, const Item& item);
}
