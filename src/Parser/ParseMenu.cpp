#include "ParseMenu.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "ParseAction.h"
#include "ParseMenuButton.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	typedef std::tuple<uint16_t, std::string, Variable> FilterObject;
	using namespace rapidjson;

	void parseFilterObj(const Value& elem, std::vector<FilterObject>& list)
	{
		if (elem.HasMember("value") == false)
		{
			return;
		}
		const auto& valElem = elem["value"];

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

	std::vector<FilterObject> parseFilterList(const Value& elem)
	{
		std::vector<FilterObject> list;
		if (elem.IsObject() == true)
		{
			parseFilterObj(elem, list);
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				if (val.IsObject() == true)
				{
					parseFilterObj(val, list);
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

	bool compareVariables(uint16_t conditionHash16,
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

	bool getFilterResult(const std::vector<FilterObject>& filterList,
		const Variable& varToCheck, bool includedOrExcluded) noexcept
	{
		bool skip = includedOrExcluded;
		for (const auto& filterObj : filterList)
		{
			if (compareVariables(std::get<0>(filterObj),
				varToCheck,
				std::get<2>(filterObj)) == true)
			{
				skip = !includedOrExcluded;
				break;
			}
		}
		return skip;
	}

	bool getFilterResult(const std::vector<FilterObject>& filterList,
		const Queryable& queryable, bool includedOrExcluded)
	{
		bool skip = includedOrExcluded;
		for (const auto& filterObj : filterList)
		{
			Variable varToCheck;
			if (queryable.getProperty(std::get<1>(filterObj), varToCheck) == true)
			{
				if (compareVariables(std::get<0>(filterObj),
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

	void parseMenu(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto menu = std::make_shared<Menu>();

		auto anchor = getAnchorKey(elem, "anchor");
		auto color = getColorKey(elem, "color", sf::Color::White);
		auto horizAlign = GameUtils::getHorizontalAlignment(getStringKey(elem, "horizontalAlign"));
		auto vertAlign = GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign"), VerticalAlign::Bottom);
		auto horizSpaceOffset = getIntKey(elem, "horizontalSpaceOffset");
		auto vertSpaceOffset = getIntKey(elem, "verticalSpaceOffset");
		auto fontSize = getUIntKey(elem, "fontSize", 12);
		auto hasFocus = getBoolKey(elem, "focus");
		auto focusOnClick = getBoolKey(elem, "focusOnClick", true);
		auto clickUp = getBoolKey(elem, "clickUp");

		sf::SoundBuffer* sound{ nullptr };
		if (elem.HasMember("sound"))
		{
			sound = game.Resources().getSoundBuffer(elem["sound"].GetString());
		}
		sf::SoundBuffer* focusSound{ nullptr };
		if (elem.HasMember("focusSound"))
		{
			focusSound = game.Resources().getSoundBuffer(elem["focusSound"].GetString());
		}

		auto font = game.Resources().getFont(getStringKey(elem, "font"));
		if (holdsNullFont(font) == true)
		{
			return;
		}

		auto relativePos = getBoolKey(elem, "relativeCoords", true);

		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
		auto origPos = getPositionKey(elem, "position", size, game.RefSize());
		auto pos = origPos;
		if (relativePos == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}

		size_t menuIdx = 0;

		auto processMenuItem = [&](const Value& val)
		{
			if (isValidString(val, "load") == true)
			{
				std::string prop(val["load"].GetString());

				auto props = Utils::splitStringIn2(prop, '.');
				const auto& uiObjId = props.first;
				const auto& uiElemProps = props.second;

				std::vector<FilterObject> filterList;
				bool include = true;

				if (val.HasMember("exclude") == true)
				{
					filterList = parseFilterList(val["exclude"]);
					include = false;
				}
				else if (val.HasMember("include") == true)
				{
					filterList = parseFilterList(val["include"]);
				}

				if (uiObjId == "game" && uiElemProps == "saves")
				{
					MemoryPoolAllocator<CrtAllocator> allocator;

					for (const auto& dir : FileUtils::getSaveDirList())
					{
						if (filterList.empty() == false &&
							getFilterResult(filterList, Variable(dir), include) == true)
						{
							continue;
						}

						Value valCopy(val, allocator);
						JsonUtils::replaceValueWithString(valCopy, allocator, "%save%", dir);

						parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
							horizSpaceOffset, vertSpaceOffset, font,
							fontSize, sound, focusSound, clickUp,
							hasFocus, focusOnClick, relativePos, origPos);

						parseAndExecuteMenuAction(game, valCopy);

						menuIdx++;
					}
				}
				else
				{
					auto level = game.Resources().getResource<Level>(uiObjId);
					if (level == nullptr)
					{
						if (uiObjId == "currentLevel")
						{
							level = game.Resources().getCurrentLevel();
						}
					}
					if (level == nullptr)
					{
						return;
					}
					if (uiElemProps == "quests")
					{
						MemoryPoolAllocator<CrtAllocator> allocator;

						const auto& quests = level->Quests();
						for (size_t i = 0; i < quests.size(); i++)
						{
							const auto& quest = quests[i];

							if (filterList.empty() == false &&
								getFilterResult(filterList, quest, include) == true)
							{
								continue;
							}

							Value valCopy(val, allocator);
							JsonUtils::replaceValueWithQueryable(valCopy, allocator, quest,
								[i, menuIdx, &allocator](Value& valFunc)
							{
								JsonUtils::replaceStringWithVariable(
									valFunc, allocator, "%idx%", Variable((int64_t)i), true);

								JsonUtils::replaceStringWithVariable(
									valFunc, allocator, "%menuIdx%", Variable((int64_t)menuIdx), true);
							});

							parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
								horizSpaceOffset, vertSpaceOffset, font,
								fontSize, sound, focusSound, clickUp,
								hasFocus, focusOnClick, relativePos, origPos);

							parseAndExecuteMenuAction(game, valCopy);

							menuIdx++;
						}
					}
					else
					{
						auto props2 = Utils::splitStringIn2(uiElemProps, '.');
						auto player = level->getPlayerOrCurrent(props2.first);
						if (player == nullptr)
						{
							return;
						}
						auto props3 = Utils::splitStringIn2(props2.second, '.');
						auto invIdx = GameUtils::getPlayerInventoryIndex(props3.second);
						if (invIdx < player->getInventorySize())
						{
							MemoryPoolAllocator<CrtAllocator> allocator;
							const auto& inventory = player->getInventory(invIdx);
							for (size_t i = 0; i < inventory.Size(); i++)
							{
								if (inventory.isSlotInUse(i) == false)
								{
									continue;
								}
								const auto item = inventory.get(i);

								if (filterList.empty() == false &&
									getFilterResult(filterList, *item, include) == true)
								{
									continue;
								}

								Value valCopy(val, allocator);
								JsonUtils::replaceValueWithQueryable(valCopy, allocator, *item,
									[i, menuIdx, &allocator](Value& valFunc)
								{
									JsonUtils::replaceStringWithVariable(
										valFunc, allocator, "%idx%", Variable((int64_t)i), true);

									JsonUtils::replaceStringWithVariable(
										valFunc, allocator, "%menuIdx%", Variable((int64_t)menuIdx), true);
								});

								parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
									horizSpaceOffset, vertSpaceOffset, font,
									fontSize, sound, focusSound, clickUp,
									hasFocus, focusOnClick, relativePos, origPos);

								parseAndExecuteMenuAction(game, valCopy);

								menuIdx++;
							}
						}
					}
				}
			}
			else
			{
				parseMenuButton(game, val, *menu, anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, font,
					fontSize, sound, focusSound, clickUp,
					hasFocus, focusOnClick, relativePos, origPos);

				parseAndExecuteMenuAction(game, val);

				menuIdx++;
			}
		};

		if (elem.HasMember("items") == true)
		{
			const auto& items = elem["items"];
			if (items.IsObject() == true)
			{
				processMenuItem(items);
			}
			else if (items.IsArray() == true)
			{
				for (const auto& val : elem["items"])
				{
					processMenuItem(val);
				}
			}
		}

		menu->setVerticalAlign(vertAlign);
		menu->setAnchor(anchor);
		menu->Size(size);
		menu->Position(pos);
		menu->ScrollPosition(GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign));
		menu->Visible(getBoolKey(elem, "visible", true));
		menu->setVerticalPad(getUIntKey(elem, "verticalPad"));
		menu->setVisibleItems(getUIntKey(elem, "visibleItems"));

		if (elem.HasMember("onScrollDown"))
		{
			menu->setAction(str2int16("scrollDown"), parseAction(game, elem["onScrollDown"]));
		}
		if (elem.HasMember("onScrollUp"))
		{
			menu->setAction(str2int16("scrollUp"), parseAction(game, elem["onScrollUp"]));
		}

		menu->updateVisibleItems();
		menu->calculatePositions();

		game.Resources().addDrawable(id, menu, getStringViewKey(elem, "resource"));
	}
}
