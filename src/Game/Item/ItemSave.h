#pragma once

#include "Game/Variable.h"
#include "Utils/UnorderedStringMap.h"

class Item;
class Level;

class ItemSave
{
public:
	// if customProp is not null, it points to the inventory index of the item (size_t)
	static void serialize(const Item& item, const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props);
};
