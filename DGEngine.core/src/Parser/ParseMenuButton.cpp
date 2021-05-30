#include "ParseMenuButton.h"
#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Menu.h"
#include "ParseAction.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseMenuButton(
		Game& game,
		const Value& val,
		Menu& menu,
		Anchor anchor,
		const sf::Color& color,
		HorizontalAlign horizAlign,
		int horizSpaceOffset,
		int vertSpaceOffset,
		const Font& font,
		unsigned fontSize,
		const sf::SoundBuffer* sound,
		const sf::SoundBuffer* focusSound,
		bool clickUp,
		bool hasFocus,
		bool focusOnClick,
		bool relativePos,
		const sf::Vector2f& origPos,
		const std::string_view resource)
	{
		std::unique_ptr<DrawableText> drawableText;
		if (holdsFreeTypeFont(font) == true)
		{
			drawableText = std::make_unique<StringText>(
				std::get<std::shared_ptr<FreeTypeFont>>(font), fontSize);
		}
		else
		{
			drawableText = std::make_unique<BitmapText>(
				std::get<std::shared_ptr<BitmapFont>>(font));
		}
		auto button = std::make_shared<StringButton>(std::move(drawableText));

		button->setAnchor(anchor);
		button->setColor(color);
		button->setHorizontalSpaceOffset(horizSpaceOffset);
		button->setVerticalSpaceOffset(vertSpaceOffset);
		button->setHorizontalAlign(horizAlign);
		button->setClickUp(clickUp);

		if (sound != nullptr)
		{
			button->setClickSound(sound);
		}
		if (focusSound != nullptr)
		{
			button->setFocusSound(focusSound);
		}

		button->enable(getBoolKey(val, "enable", true));
		button->setText(getStringKey(val, "text"));

		auto pos2 = getVector2fKey<sf::Vector2f>(val, "position", origPos);
		if (relativePos == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			auto size = button->Size();
			GameUtils::setAnchorPosSize(anchor, pos2, size, game.RefSize(), game.DrawRegionSize());
		}
		button->Position(pos2);

		if (val.HasMember("onClick"sv))
		{
			button->setAction(str2int16("click"), getActionVal(game, val["onClick"sv]));
		}
		if (val.HasMember("onDoubleClick"sv))
		{
			button->setAction(str2int16("doubleClick"), getActionVal(game, val["onDoubleClick"sv]));
		}
		if (val.HasMember("onClickIn"sv))
		{
			button->setAction(str2int16("clickIn"), getActionVal(game, val["onClickIn"sv]));
		}
		if (val.HasMember("onClickOut"sv))
		{
			button->setAction(str2int16("clickOut"), getActionVal(game, val["onClickOut"sv]));
		}
		if (val.HasMember("onFocus"sv))
		{
			button->setAction(str2int16("focus"), getActionVal(game, val["onFocus"sv]));
		}
		if (val.HasMember("onHoverEnter"sv))
		{
			button->setAction(str2int16("hoverEnter"), getActionVal(game, val["onHoverEnter"sv]));
		}
		if (val.HasMember("onHoverLeave"sv))
		{
			button->setAction(str2int16("hoverLeave"), getActionVal(game, val["onHoverLeave"sv]));
		}

		menu.addItem(button);

		if (hasFocus == true)
		{
			button->focusEnabled(focusOnClick);
			game.Resources().addFocused(button, resource);
		}
	}
}
