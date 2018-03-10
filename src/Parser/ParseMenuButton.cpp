#include "ParseMenuButton.h"
#include "BitmapText.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

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
		const sf::Vector2f& origPos)
	{
		auto button = std::make_shared<StringButton>();
		std::unique_ptr<DrawableText> drawableText;
		if (hasFreeTypeFont(font) == true)
		{
			drawableText = std::make_unique<StringText>("",
				*std::get<std::shared_ptr<FreeTypeFont>>(font), fontSize);
		}
		else
		{
			drawableText = std::make_unique<BitmapText>("",
				std::get<std::shared_ptr<BitmapFont>>(font), horizSpaceOffset, vertSpaceOffset);
		}
		button->setText(std::move(drawableText));

		button->setAnchor(anchor);
		button->setColor(color);
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
			button->setAction(str2int16("click"), parseAction(game, val["onClick"]));
		}
		if (val.HasMember("onDoubleClick"))
		{
			button->setAction(str2int16("doubleClick"), parseAction(game, val["onDoubleClick"]));
		}
		if (val.HasMember("onClickIn"))
		{
			button->setAction(str2int16("clickIn"), parseAction(game, val["onClickIn"]));
		}
		if (val.HasMember("onClickOut"))
		{
			button->setAction(str2int16("clickOut"), parseAction(game, val["onClickOut"]));
		}
		if (val.HasMember("onFocus"))
		{
			button->setAction(str2int16("focus"), parseAction(game, val["onFocus"]));
		}
		if (val.HasMember("onHoverEnter"))
		{
			button->setAction(str2int16("hoverEnter"), parseAction(game, val["onHoverEnter"]));
		}
		if (val.HasMember("onHoverLeave"))
		{
			button->setAction(str2int16("hoverLeave"), parseAction(game, val["onHoverLeave"]));
		}

		menu.addItem(button);

		if (hasFocus == true)
		{
			button->focusEnabled(focusOnClick);
			game.Resources().addFocused(button);
		}
	};
}
