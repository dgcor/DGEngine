#include "ParseMenu.h"
#include "BitmapText.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "ParseMenuButton.h"
#include "ParseMenuQuests.h"
#include "ParseMenuSaveGames.h"
#include "ParseUtils.h"
#include "StringButton.h"
#include "StringText.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMenu(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			isValidArray(elem, "items") == false)
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
			if (val.HasMember("%QUESTS%"))
			{
				parseMenuQuests(game, val, *menu, pos, anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, isTextFont, font, fontSize,
					bitmapFont, sound, focusSound, clickUp, hasFocus, focusOnClick);
			}
			else if (val.HasMember("%SAVEGAMES%"))
			{
				parseMenuSaveGames(game, val, *menu, pos, anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, isTextFont, font, fontSize,
					bitmapFont, sound, focusSound, clickUp, hasFocus, focusOnClick);
			}
			else
			{
				auto button = parseMenuButton(anchor, color, horizAlign,
					horizSpaceOffset, vertSpaceOffset, isTextFont, font,
					fontSize, bitmapFont, sound, focusSound, clickUp);
				button->enable(getBoolKey(val, "enable", true));
				button->setText(getStringKey(val, "text"));

				auto pos2 = getVector2fKey<sf::Vector2f>(val, "position", origPos);
				if (relativePos == true)
				{
					auto size = button->Size();
					GameUtils::setAnchorPosSize(anchor, pos2, size, game.RefSize(), game.MinSize());
					if (game.StretchToFit() == false)
					{
						GameUtils::setAnchorPosSize(anchor, pos2, size, game.MinSize(), game.WindowSize());
					}
				}
				button->Position(pos2);

				if (val.HasMember("onClick"))
				{
					button->setClickAction(parseAction(game, val["onClick"]));
				}

				if (val.HasMember("onDoubleClick"))
				{
					button->setDoubleClickAction(parseAction(game, val["onDoubleClick"]));
				}

				if (val.HasMember("onClickIn"))
				{
					button->setClickInAction(parseAction(game, val["onClickIn"]));
				}

				if (val.HasMember("onClickOut"))
				{
					button->setClickOutAction(parseAction(game, val["onClickOut"]));
				}

				if (val.HasMember("onFocus"))
				{
					button->setFocusAction(parseAction(game, val["onFocus"]));
				}

				if (val.HasMember("onHoverEnter"))
				{
					button->setHoverEnterAction(parseAction(game, val["onHoverEnter"]));
				}

				if (val.HasMember("onHoverLeave"))
				{
					button->setHoverLeaveAction(parseAction(game, val["onHoverLeave"]));
				}

				menu->addItem(button);

				if (hasFocus == true)
				{
					button->focusEnabled(focusOnClick);
					game.Resources().addFocused(button);
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
			menu->setScrollDownAction(parseAction(game, elem["onScrollDown"]));
		}

		if (elem.HasMember("onScrollUp"))
		{
			menu->setScrollUpAction(parseAction(game, elem["onScrollUp"]));
		}

		menu->calculatePositions();

		game.Resources().addDrawable(elem["id"].GetString(), menu);
	}
}
