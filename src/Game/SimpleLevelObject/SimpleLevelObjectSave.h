#pragma once

#include "Game/Variable.h"
#include "Utils/UnorderedStringMap.h"

class Level;
class SimpleLevelObject;

class SimpleLevelObjectSave
{
public:
	static void serialize(const SimpleLevelObject& obj, const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props);
};
