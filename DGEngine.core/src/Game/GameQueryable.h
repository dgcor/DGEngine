#pragma once

#include "Action.h"
#include <string_view>
#include "VarOrQueryObject.h"
#include <vector>

class GameQueryable
{
public:
	static std::shared_ptr<Action> getAction(const Game& game, uint16_t nameHash16) noexcept;

	static bool setAction(Game& game, uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	// get an action from a UIObject using a query.
	// ex: getQueryAction("btn1.click")
	static std::shared_ptr<Action> getQueryAction(const Game& game, const std::string_view prop);

	static bool getProperty(const Game& game, const std::string_view prop, Variable& var);

	static bool getProperty(const Game& game, const std::string_view prop1, const std::string_view prop2, Variable& var);

	static bool getGameProperty(const Game& game, const std::string_view prop, Variable& var);

	static bool getGameProperty(const Game& game, const std::string_view prop1, const std::string_view prop2, Variable& var);

	static void setGameProperty(Game& game, const std::string_view prop, const Variable& val);

	static QueryObject getQueryable(const Game& game, const std::string_view prop);

	static QueryObject getQueryable(const Game& game, const std::string_view prop1, const std::string_view prop2);

	static std::vector<VarOrQueryObject> getQueryableList(const Game& game, const std::string_view prop);

	static bool getQueryableList(const std::string_view prop1,
		const std::string_view prop2, std::vector<VarOrQueryObject>& queryableList);
};
