#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Variable.h"
#include <vector>

class Game;
class Menu;
class Queryable;

namespace Parser
{
	typedef std::tuple<uint16_t, std::string, Variable> MenuFilterObject;

	void parseMenuFilterObj(const rapidjson::Value& elem, std::vector<MenuFilterObject>& list);

	std::vector<MenuFilterObject> parseMenuFilterList(const rapidjson::Value& elem);

	void parseAndExecuteMenuAction(Game& game, const rapidjson::Value& elem);

	bool compareMenuFilterVariables(uint16_t conditionHash16,
		const Variable& var1, const Variable& var2) noexcept;

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Variable& varToCheck, bool includedOrExcluded) noexcept;

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Queryable& queryable, bool includedOrExcluded);

	std::shared_ptr<Menu> getMenuObj(Game& game, const rapidjson::Value& elem,
		const std::string_view resource);

	typedef decltype(&getMenuObj) getMenuObjFuncPtr;

	void parseMenu(Game& game, const rapidjson::Value& elem,
		const getMenuObjFuncPtr getMenuObjFunc);

	void parseMenu(Game& game, const rapidjson::Value& elem);
}
