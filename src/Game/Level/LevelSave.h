#pragma once

#include "Game/Variable.h"
#include <string_view>
#include "Utils/UnorderedStringMap.h"

class Game;
class Level;

class LevelSave
{
private:
	static void serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props);

public:
	static void save(const Level& level, const std::string_view filePath, const UnorderedStringMap<Variable>& props);
};
