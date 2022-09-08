#pragma once

#include "Game/Properties/LevelObjValue.h"
#include "Game/VarOrQueryObject.h"
#include <memory>
#include "Utils/Number.h"
#include <vector>

class PlayerBase;

class PlayerLevelObject
{
public:
	static bool getNumber(const PlayerBase& player, const std::string_view prop, Number32& value);

	static bool getNumberByHash(const PlayerBase& player, uint16_t propHash, const std::string_view props, Number32& value);

	static bool getNumberByHash(const PlayerBase& player, uint16_t propHash, Number32& value);

	static bool getNumberByHash(const PlayerBase& player, uint16_t propHash, LevelObjValue& value);

	static bool getProperty(const PlayerBase& player, const std::string_view prop, Variable& var);

	static bool setProperty(PlayerBase& player, const std::string_view prop, const Variable& var);

	static QueryObject getQueryable(const PlayerBase& player, const std::string_view prop);

	static std::vector<VarOrQueryObject> getQueryableList(const PlayerBase& player, const std::string_view prop);
};
