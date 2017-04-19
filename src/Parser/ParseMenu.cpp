#include "ParseMenu.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "ParseAction.h"
#include "ParseMenuButton.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

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

		auto conditionHash = str2int16(getStringCharKey(elem, "if"));
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
		const Variable& var1, const Variable& var2)
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
		const Variable& varToCheck, bool includedOrExcluded)
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
		auto vertAlign = GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign"));
		auto horizSpaceOffset = getIntKey(elem, "horizontalSpaceOffset");
		auto vertSpaceOffset = getIntKey(elem, "verticalSpaceOffset");
		auto fontSize = getUIntKey(elem, "fontSize");
		auto hasFocus = getBoolKey(elem, "focus");
		auto focusOnClick = getBoolKey(elem, "focusOnClick", true);
		auto clickUp = getBoolKey(elem, "clickUp");

		std::shared_ptr<sf::SoundBuffer> sound;
		if (elem.HasMember("sound"))
		{
			sound = game.Resources().getSound(elem["sound"].GetString());
		}
		std::shared_ptr<sf::SoundBuffer> focusSound;
		if (elem.HasMember("focusSound"))
		{
			focusSound = game.Resources().getSound(elem["focusSound"].GetString());
		}

		auto isTextFont = elem.HasMember("font");
		std::shared_ptr<Font2> font;
		std::shared_ptr<BitmapFont> bitmapFont;
		if (isTextFont == true)
		{
			font = game.Resources().getFont(getStringVal(elem["font"]));
			if (font == nullptr)
			{
				return;
			}
		}
		else
		{
			bitmapFont = game.Resources().getBitmapFont(getStringKey(elem, "bitmapFont"));
			if (bitmapFont == nullptr)
			{
				return;
			}
		}

		auto relativePos = getBoolKey(elem, "relativeCoords", true);

		auto origPos = getVector2fKey<sf::Vector2f>(elem, "position");
		auto pos = origPos;
		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
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

				auto props = Utils::splitStringIn2(prop, '|');
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
							horizSpaceOffset, vertSpaceOffset, isTextFont, font,
							fontSize, bitmapFont, sound, focusSound, clickUp,
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
								horizSpaceOffset, vertSpaceOffset, isTextFont, font,
								fontSize, bitmapFont, sound, focusSound, clickUp,
								hasFocus, focusOnClick, relativePos, origPos);

							parseAndExecuteMenuAction(game, valCopy);

							menuIdx++;
						}
					}
					else
					{
						auto props2 = Utils::splitStringIn2(uiElemProps, '.');
						auto player = level->getPlayer(props2.first);
						if (player == nullptr)
						{
							player = level->getCurrentPlayer();
						}
						if (player == nullptr)
						{
							return;
						}
						auto props3 = Utils::splitStringIn2(props2.second, '.');
						auto invIdx = GameUtils::getPlayerInventoryIndex(props3.second);
						if (invIdx < player->getInventorySize())
						{
							MemoryPoolAllocator<CrtAllocator> allocator;
							const auto& itemCollection = player->getInventory(invIdx);
							for (size_t i = 0; i < itemCollection.Size(); i++)
							{
								if (itemCollection.isItemSlotInUse(i) == false)
								{
									continue;
								}
								const auto& item = itemCollection[i];

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
									horizSpaceOffset, vertSpaceOffset, isTextFont, font,
									fontSize, bitmapFont, sound, focusSound, clickUp,
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
					horizSpaceOffset, vertSpaceOffset, isTextFont, font,
					fontSize, bitmapFont, sound, focusSound, clickUp,
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

		game.Resources().addDrawable(id, menu);
	}
}
