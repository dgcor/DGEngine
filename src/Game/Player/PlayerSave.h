#pragma once

#include "Game/Variable.h"
#include "Utils/UnorderedStringMap.h"

class Level;
class PlayerBase;

class PlayerSave
{
public:
	static void serialize(const PlayerBase& player, const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props);
};
