#include "ParseMenuItem.h"
#include "Game/Drawables/Menu.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "Game/Utils/VarUtils.h"
#include "Json/JsonUtils.h"
#include "ParseButton.h"
#include "ParseText.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseMenuFilterObj(const Value& elem, std::vector<MenuFilterObject>& list)
	{
		if (elem.HasMember("value"sv) == false)
		{
			return;
		}
		const auto& valElem = elem["value"sv];

		auto conditionHash = str2int16(getStringViewKey(elem, "if"));
		auto property = getStringKey(elem, "property");

		if (valElem.IsArray() == false)
		{
			list.push_back(std::make_tuple(conditionHash,
				property,
				getVariableVal(valElem)));
		}
		else
		{
			for (const auto& val : valElem)
			{
				list.push_back(std::make_tuple(conditionHash,
					property,
					getVariableVal(val)));
			}
		}
	}

	std::vector<MenuFilterObject> parseMenuFilterList(const Value& elem)
	{
		std::vector<MenuFilterObject> list;
		if (elem.IsObject() == true)
		{
			parseMenuFilterObj(elem, list);
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				if (val.IsObject() == true)
				{
					parseMenuFilterObj(val, list);
				}
			}
		}
		return list;
	}

	void parseAndExecuteMenuAction(Game& game, const Value& elem)
	{
		game.Events().tryAddBack(getActionKey(game, elem, "executeAction"));
	}

	bool compareMenuFilterVariables(uint16_t conditionHash16, const Variable& var1, const Variable& var2) noexcept
	{
		switch (conditionHash16)
		{
		default:
		case str2int16("=="):
			return var1 == var2;
		case str2int16("!="):
			return var1 != var2;
		case str2int16(">"):
			return var1 > var2;
		case str2int16(">="):
			return var1 >= var2;
		case str2int16("<"):
			return var1 < var2;
		case str2int16("<="):
			return var1 <= var2;
		}
	}

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Variable& varToCheck, bool includedOrExcluded) noexcept
	{
		bool skip = includedOrExcluded;
		for (const auto& filterObj : filterList)
		{
			if (compareMenuFilterVariables(std::get<0>(filterObj),
				varToCheck,
				std::get<2>(filterObj)) == true)
			{
				skip = !includedOrExcluded;
				break;
			}
		}
		return skip;
	}

	bool getMenuFilterResult(const std::vector<MenuFilterObject>& filterList,
		const Queryable& queryable, bool includedOrExcluded)
	{
		bool skip = includedOrExcluded;
		for (const auto& filterObj : filterList)
		{
			Variable varToCheck;
			if (queryable.getProperty(std::get<1>(filterObj), varToCheck) == true)
			{
				if (compareMenuFilterVariables(std::get<0>(filterObj),
					varToCheck,
					std::get<2>(filterObj)) == true)
				{
					skip = !includedOrExcluded;
					break;
				}
			}
		}
		return skip;
	}

	void parseMenuButton(Game& game, const Value& elem, Menu& menu,
		StringButton& referenceButton, bool relativePos, const std::string_view resource)
	{
		auto button = std::make_shared<StringButton>(referenceButton);

		button->enable(getBoolKey(elem, "enable", referenceButton.isEnabled()));

		parseDrawableTextDisplayText(elem, *button->getDrawableText());
		parseBindableTextObj(game, elem, *button);

		bool recalculatePosition = elem.HasMember("position"sv) == false;
		if (recalculatePosition == false)
		{
			auto pos = getVector2fVal<sf::Vector2f>(elem["position"sv], referenceButton.Position());
			if (relativePos == true &&
				game.RefSize() != game.DrawRegionSize())
			{
				auto size = button->Size();
				GameUtils::setAnchorPosSize(referenceButton.getAnchor(), pos, size, game.RefSize(), game.DrawRegionSize());
			}
			button->Position(pos);
		}

		parseButtonActions(game, elem, *button);

		menu.addItem(button, recalculatePosition);

		if (button->isFocusEnabled() == true)
		{
			game.Resources().addFocused(button, resource);
		}
	}

	size_t parseMenuItem(Game& game, const Value& elem, Menu& menu, size_t menuIdx,
		StringButton& referenceButton, bool relativePos, const std::string_view resource)
	{
		MemoryPoolAllocator<CrtAllocator> allocator;

		if (isValidString(elem, "load") == true)
		{
			std::vector<MenuFilterObject> filterList;
			bool include = true;

			if (elem.HasMember("exclude"sv) == true)
			{
				filterList = parseMenuFilterList(elem["exclude"sv]);
				include = false;
			}
			else if (elem.HasMember("include"sv) == true)
			{
				filterList = parseMenuFilterList(elem["include"sv]);
			}

			auto menuItems = game.getQueryableList(getStringViewVal(elem["load"sv]));

			for (size_t i = 0; i < menuItems.size(); i++)
			{
				const auto& menuItem = menuItems[i];
				if (holdsQueryObject(menuItem) == true)
				{
					auto queryable = getQueryablePtr(std::get<QueryObject>(menuItem));
					if (queryable == nullptr)
					{
						continue;
					}
					if (filterList.empty() == false &&
						getMenuFilterResult(filterList, *queryable, include) == true)
					{
						continue;
					}
				}
				else
				{
					const auto& var = std::get<Variable>(menuItem);
					if (filterList.empty() == false &&
						getMenuFilterResult(filterList, var, include) == true)
					{
						continue;
					}
				}

				Value elemCopy(elem, allocator);

				if (holdsQueryObject(menuItem) == true)
				{
					const auto& queryable = *getQueryablePtr(std::get<QueryObject>(menuItem));
					auto listIdx = getQueryablePtrIndex(std::get<QueryObject>(menuItem), i);

					JsonUtils::replaceValuesWithFunction(elemCopy, allocator,
						[&queryable, listIdx, menuIdx, &allocator](Value& val)
						{
							JsonUtils::replaceStringValueWithQueryable(val, allocator, queryable, true);

							JsonUtils::replaceStringValueWithVariable(
								val, allocator, "!listIdx!", Variable((int64_t)listIdx), true);

							JsonUtils::replaceStringValueWithVariable(
								val, allocator, "!menuIdx!", Variable((int64_t)menuIdx), true);
						});
				}
				else
				{
					const auto& var = std::get<Variable>(menuItem);

					JsonUtils::replaceValuesWithFunction(elemCopy, allocator,
						[var, menuIdx, &allocator](Value& val)
						{
							JsonUtils::replaceStringValueWithVariable(
								val, allocator, "!listItem!", VarUtils::toString(var));

							JsonUtils::replaceStringValueWithVariable(
								val, allocator, "!menuIdx!", Variable((int64_t)menuIdx), true);
						});
				}

				parseMenuButton(game, elemCopy, menu, referenceButton, relativePos, resource);
				parseAndExecuteMenuAction(game, elemCopy);
				menuIdx++;
			}
		}
		else
		{
			Value elemCopy(elem, allocator);

			JsonUtils::replaceValuesWithFunction(elemCopy, allocator,
				[menuIdx, &allocator](Value& val)
				{
					JsonUtils::replaceStringValueWithVariable(
						val, allocator, "!menuIdx!", Variable((int64_t)menuIdx), true);
				});

			parseMenuButton(game, elemCopy, menu, referenceButton, relativePos, resource);
			parseAndExecuteMenuAction(game, elemCopy);
			menuIdx++;
		}
		return menuIdx;
	}
}
