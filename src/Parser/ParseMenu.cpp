#include "ParseMenu.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "ParseMenuButton.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	typedef std::tuple<unsigned, std::string, Variable> ExcludeObject;
	using namespace rapidjson;

	void parseExcludeObj(const Value& elem, std::vector<ExcludeObject>& list)
	{
		if (elem.HasMember("value") == false)
		{
			return;
		}
		const auto& valElem = elem["value"];

		auto conditionHash = str2int32(getStringCharKey(elem, "if"));
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

	std::vector<ExcludeObject> parseExcludeList(const Value& elem)
	{
		std::vector<ExcludeObject> list;
		if (elem.IsObject() == true)
		{
			parseExcludeObj(elem, list);
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				if (val.IsObject() == true)
				{
					parseExcludeObj(val, list);
				}
			}
		}
		return list;
	}

	bool compareVariables(unsigned conditionHash,
		const Variable& var1, const Variable& var2)
	{
		switch (conditionHash)
		{
		default:
		case str2int32("=="):
			return var1 == var2;
		case str2int32("!="):
			return var1 != var2;
		case str2int32(">"):
			return var1 > var2;
		case str2int32(">="):
			return var1 >= var2;
		case str2int32("<"):
			return var1 < var2;
		case str2int32("<="):
			return var1 <= var2;
		}
	}

	void parseMenu(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			isValidArray(elem, "items") == false)
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

		for (const auto& val : elem["items"])
		{
			if (isValidString(val, "load") == true)
			{
				std::string prop(val["load"].GetString());

				auto props = Utils::splitStringIn2(prop, '|');
				const auto& uiObjId = props.first;
				const auto& uiElemProps = props.second;

				std::vector<ExcludeObject> excludeList;

				if (val.HasMember("exclude") == true)
				{
					excludeList = parseExcludeList(val["exclude"]);
				}

				if (uiObjId == "game" && uiElemProps == "saves")
				{
					MemoryPoolAllocator<CrtAllocator> allocator;

					for (const auto& dir : FileUtils::getSaveDirList())
					{
						bool skip = false;
						for (const auto& excludeObj : excludeList)
						{
							if (compareVariables(std::get<0>(excludeObj),
								Variable(dir),
								std::get<2>(excludeObj)) == true)
							{
								skip = true;
								break;
							}
						}
						if (skip == true)
						{
							continue;
						}

						Value valCopy(val, allocator);
						replaceValWithString(valCopy, allocator, "%save%", dir);

						parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
							horizSpaceOffset, vertSpaceOffset, isTextFont, font,
							fontSize, bitmapFont, sound, focusSound, clickUp,
							hasFocus, focusOnClick, relativePos, origPos);
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
						continue;
					}

					if (uiElemProps == "quests")
					{
						MemoryPoolAllocator<CrtAllocator> allocator;

						for (const auto& quest : level->Quests())
						{
							bool skip = false;
							for (const auto& excludeObj : excludeList)
							{
								Variable ignorePropVar;
								if (quest.getProperty(std::get<1>(excludeObj), ignorePropVar) == true)
								{
									if (compareVariables(std::get<0>(excludeObj),
										ignorePropVar,
										std::get<2>(excludeObj)) == true)
									{
										skip = true;
										break;
									}
								}
							}
							if (skip == true)
							{
								continue;
							}

							Value valCopy(val, allocator);
							replaceValWithQueryable(valCopy, allocator, quest);

							parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
								horizSpaceOffset, vertSpaceOffset, isTextFont, font,
								fontSize, bitmapFont, sound, focusSound, clickUp,
								hasFocus, focusOnClick, relativePos, origPos);
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
							continue;
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
								bool skip = false;
								for (const auto& excludeObj : excludeList)
								{
									Variable ignorePropVar;
									if (item->getProperty(std::get<1>(excludeObj), ignorePropVar) == true)
									{
										if (compareVariables(std::get<0>(excludeObj),
											ignorePropVar,
											std::get<2>(excludeObj)) == true)
										{
											skip = true;
											break;
										}
									}
								}
								if (skip == true)
								{
									continue;
								}

								Value valCopy(val, allocator);
								replaceValWithQueryable(valCopy, allocator, *item);

								parseMenuButton(game, valCopy, *menu, anchor, color, horizAlign,
									horizSpaceOffset, vertSpaceOffset, isTextFont, font,
									fontSize, bitmapFont, sound, focusSound, clickUp,
									hasFocus, focusOnClick, relativePos, origPos);
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

		menu->calculatePositions();

		game.Resources().addDrawable(id, menu);
	}
}
