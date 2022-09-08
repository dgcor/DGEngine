#pragma once

#include "Game/Queryable.h"
#include "Game/Variable.h"
#include "Json/JsonParser.h"
#include <string_view>
#include <vector>

class Game;
class Menu;
class StringButton;

namespace Parser
{
	using MenuFilterObject = std::tuple<uint16_t, std::string, Variable>;

	void parseMenuFilterObj(const rapidjson::Value& elem, std::vector<MenuFilterObject>& list);

	std::vector<MenuFilterObject> parseMenuFilterList(const rapidjson::Value& elem);

	void parseAndExecuteMenuAction(Game& game, const rapidjson::Value& elem);

	bool compareMenuFilterVariables(uint16_t conditionHash16,
		const Variable& var1, const Variable& var2) noexcept;

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Variable& varToCheck, bool includedOrExcluded) noexcept;

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Queryable& queryable, bool includedOrExcluded);

	void parseMenuButton(Game& game, const rapidjson::Value& elem, Menu& menu,
		StringButton& referenceButton, bool relativePos, const std::string_view resource);

	// returns the next menuIdx
	size_t parseMenuItem(Game& game, const rapidjson::Value& elem, Menu& menu, size_t menuIdx,
		StringButton& referenceButton, bool relativePos, const std::string_view resource);
}
