#pragma once

#include "Game/VarOrQueryObject.h"
#include <string_view>
#include <vector>

class Action;
class Level;

class LevelUIObject
{
public:
	static std::shared_ptr<Action> getAction(const Level& level, uint16_t nameHash16) noexcept;

	static bool setAction(Level& level, uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	static bool getProperty(const Level& level, const std::string_view prop, Variable& var);

	static QueryObject getQueryable(const Level& level, const std::string_view prop);

	static std::vector<VarOrQueryObject> getQueryableList(const Level& level, const std::string_view prop);
};
