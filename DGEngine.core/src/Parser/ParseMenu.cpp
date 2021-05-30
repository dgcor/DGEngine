#include "ParseMenu.h"
#include "BitmapText.h"
#include <cassert>
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Menu.h"
#include "Panel.h"
#include "ParseAction.h"
#include "ParseMenuButton.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

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
		auto action = getActionKey(game, elem, "executeAction");
		if (action != nullptr)
		{
			game.Events().addBack(action);
		}
	}

	bool compareMenuFilterVariables(uint16_t conditionHash16,
		const Variable& var1, const Variable& var2) noexcept
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

	std::shared_ptr<Menu> getMenuObj(Game& game, const Value& elem,
		const std::string_view resource)
	{
		auto menu = std::make_shared<Menu>();

		auto anchor = getAnchorKey(elem, "anchor");
		auto color = getColorKey(elem, "color", sf::Color::White);
		auto horizAlign = GameUtils::getHorizontalAlignment(getStringViewKey(elem, "horizontalAlign"));
		auto vertAlign = GameUtils::getVerticalAlignment(getStringViewKey(elem, "verticalAlign"), VerticalAlign::Bottom);
		auto horizSpaceOffset = getIntKey(elem, "horizontalSpaceOffset");
		auto vertSpaceOffset = getIntKey(elem, "verticalSpaceOffset");
		auto fontSize = getUIntKey(elem, "fontSize", 12);
		auto hasFocus = getBoolKey(elem, "focus");
		auto focusOnClick = getBoolKey(elem, "focusOnClick", true);
		auto clickUp = getBoolKey(elem, "clickUp");

		sf::SoundBuffer* sound{ nullptr };
		if (isValidString(elem, "sound"))
		{
			sound = game.Resources().getSoundBuffer(elem["sound"sv].GetStringView());
		}
		sf::SoundBuffer* focusSound{ nullptr };
		if (isValidString(elem, "focusSound"))
		{
			focusSound = game.Resources().getSoundBuffer(elem["focusSound"sv].GetStringView());
		}

		auto font = game.Resources().getFont(getStringViewKey(elem, "font"));
		if (holdsNullFont(font) == true)
		{
			return nullptr;
		}

		auto relativePos = getBoolKey(elem, "relativeCoords", true);

		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
		auto origPos = getPositionKey(elem, "position", size, game.RefSize());
		auto pos = origPos;
		if (relativePos == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}

		size_t menuIdx = 0;

		auto processMenuItem = [&](const Value& val)
		{
			if (isValidString(val, "load") == true)
			{
				std::vector<MenuFilterObject> filterList;
				bool include = true;

				if (val.HasMember("exclude"sv) == true)
				{
					filterList = parseMenuFilterList(val["exclude"sv]);
					include = false;
				}
				else if (val.HasMember("include"sv) == true)
				{
					filterList = parseMenuFilterList(val["include"sv]);
				}

				MemoryPoolAllocator<CrtAllocator> allocator;

				auto menuItems = game.getQueryableList(getStringViewVal(val["load"sv]));

				for (size_t i = 0; i < menuItems.size(); i++)
				{
					const auto& menuItem = menuItems[i];
					if (std::holds_alternative<const Queryable*>(menuItem) == true)
					{
						auto queryable = std::get<const Queryable*>(menuItem);
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

					Value valCopy(val, allocator);

					if (std::holds_alternative<const Queryable*>(menuItem) == true)
					{
						auto queryable = std::get<const Queryable*>(menuItem);
						JsonUtils::replaceValueWithQueryable(valCopy, allocator, *queryable,
							[i, menuIdx, &allocator](Value& valFunc)
							{
								JsonUtils::replaceStringWithVariable(
									valFunc, allocator, "!listIdx!", Variable((int64_t)i), true);

								JsonUtils::replaceStringWithVariable(
									valFunc, allocator, "!menuIdx!", Variable((int64_t)menuIdx), true);
							});
					}
					else
					{
						const auto& var = std::get<Variable>(menuItem);
						JsonUtils::replaceValueWithString(
							valCopy, allocator, "!listItem!", VarUtils::toString(var));
					}

					parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
						horizSpaceOffset, vertSpaceOffset, font,
						fontSize, sound, focusSound, clickUp, hasFocus,
						focusOnClick, relativePos, origPos, resource);

					parseAndExecuteMenuAction(game, valCopy);

					menuIdx++;
				}
			}
			else
			{
				parseMenuButton(game, val, *menu, anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, font,
					fontSize, sound, focusSound, clickUp, hasFocus,
					focusOnClick, relativePos, origPos, resource);

				parseAndExecuteMenuAction(game, val);

				menuIdx++;
			}
		};

		if (elem.HasMember("items"sv) == true)
		{
			const auto& items = elem["items"sv];
			if (items.IsObject() == true)
			{
				processMenuItem(items);
			}
			else if (items.IsArray() == true)
			{
				for (const auto& val : elem["items"sv])
				{
					processMenuItem(val);
				}
			}
		}

		if (hasFocus == true)
		{
			menu->focusEnabled();
		}
		menu->setVerticalAlign(vertAlign);
		menu->setAnchor(anchor);
		menu->Size(size);
		menu->Position(pos);
		menu->ScrollPosition(GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign));
		menu->Visible(getBoolKey(elem, "visible", true));
		menu->setVerticalPad(getUIntKey(elem, "verticalPad"));
		menu->setVisibleItems(getUIntKey(elem, "visibleItems"));

		if (elem.HasMember("onScrollDown"sv))
		{
			menu->setAction(str2int16("scrollDown"), getActionVal(game, elem["onScrollDown"sv]));
		}
		if (elem.HasMember("onScrollUp"sv))
		{
			menu->setAction(str2int16("scrollUp"), getActionVal(game, elem["onScrollUp"sv]));
		}

		menu->updateVisibleItems();
		menu->calculatePositions();

		return menu;
	}

	void parseMenu(Game& game, const Value& elem,
		const getMenuObjFuncPtr getMenuObjFunc)
	{
		assert(getMenuObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}
		auto resource = getStringViewKey(elem, "resource");

		auto menu = getMenuObjFunc(game, elem, resource);
		if (menu == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(menu);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, menu, manageObjDrawing, resource
		);
	}

	void parseMenu(Game& game, const rapidjson::Value& elem)
	{
		parseMenu(game, elem, getMenuObj);
	}
}
