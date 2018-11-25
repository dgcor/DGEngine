#include "ParseButton.h"
#include "BitmapButton.h"
#include "BitmapText.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Button> parseBitmapButton(Game& game, const Value& elem)
	{
		if (isValidString(elem, "texture") == false)
		{
			return nullptr;
		}

		auto button = std::make_shared<BitmapButton>();

		auto tex = game.Resources().getTexture(elem["texture"].GetString());
		if (tex == nullptr)
		{
			return nullptr;
		}

		button->setTexture(*tex.get(), true);

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, game.WindowSize().x, game.WindowSize().y);
			button->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}
		button->setResizable(getBoolKey(elem, "resizable"));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		button->setOutline(outline, outlineIgnore);
		button->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		return button;
	}

	std::shared_ptr<Button> parseStringButton(Game& game, const Value& elem)
	{
		auto button = std::make_shared<StringButton>();
		if (parseText2Obj(game, elem, *button) == false)
		{
			return nullptr;
		}
		return button;
	}

	void parseButton(Game& game, const Value& elem)
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

		std::shared_ptr<Button> button;

		if (elem.HasMember("texture") == true)
		{
			button = parseBitmapButton(game, elem);
			if (button != nullptr)
			{
				auto anchor = getAnchorKey(elem, "anchor");
				button->setAnchor(anchor);
				auto size = button->Size();
				auto pos = getPositionKey(elem, "position", size, game.RefSize());
				if (getBoolKey(elem, "relativeCoords", true) == true)
				{
					GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
					if (game.StretchToFit() == false)
					{
						GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
					}
				}
				button->Position(pos);
				auto bitmapBtn = static_cast<BitmapButton*>(button.get());
				if (bitmapBtn->getResizable() == true)
				{
					bitmapBtn->Size(size);
				}
				button->Visible(getBoolKey(elem, "visible", true));

				button->setColor(getColorKey(elem, "color", sf::Color::White));
			}
		}
		if (button == nullptr)
		{
			button = parseStringButton(game, elem);
			if (button == nullptr)
			{
				return;
			}
		}

		button->enable(getBoolKey(elem, "enable", true));
		button->setClickUp(getBoolKey(elem, "clickUp"));
		button->setCaptureInputEvents(getInputEventKey(elem, "captureInputEvents"));

		if (elem.HasMember("onChange"))
		{
			button->setAction(str2int16("change"), parseAction(game, elem["onChange"]));
		}
		if (elem.HasMember("onClick"))
		{
			button->setAction(str2int16("click"), parseAction(game, elem["onClick"]));
		}
		if (elem.HasMember("onRightClick"))
		{
			button->setAction(str2int16("rightClick"), parseAction(game, elem["onRightClick"]));
		}
		if (elem.HasMember("onDoubleClick"))
		{
			button->setAction(str2int16("doubleClick"), parseAction(game, elem["onDoubleClick"]));
		}
		if (elem.HasMember("onClickDrag"))
		{
			button->setAction(str2int16("clickDrag"), parseAction(game, elem["onClickDrag"]));
		}
		if (elem.HasMember("onClickIn"))
		{
			button->setAction(str2int16("clickIn"), parseAction(game, elem["onClickIn"]));
		}
		if (elem.HasMember("onClickOut"))
		{
			button->setAction(str2int16("clickOut"), parseAction(game, elem["onClickOut"]));
		}
		if (elem.HasMember("onFocus"))
		{
			button->setAction(str2int16("focus"), parseAction(game, elem["onFocus"]));
		}
		if (elem.HasMember("onHoverEnter"))
		{
			button->setAction(str2int16("hoverEnter"), parseAction(game, elem["onHoverEnter"]));
		}
		if (elem.HasMember("onHoverLeave"))
		{
			button->setAction(str2int16("hoverLeave"), parseAction(game, elem["onHoverLeave"]));
		}
		if (elem.HasMember("sound"))
		{
			button->setClickSound(game.Resources().getSoundBuffer(elem["sound"].GetString()));
		}
		if (elem.HasMember("focusSound"))
		{
			button->setFocusSound(game.Resources().getSoundBuffer(elem["focusSound"].GetString()));
		}

		game.Resources().addDrawable(id, button, getStringViewKey(elem, "resource"));

		if (getBoolKey(elem, "focus") == true)
		{
			button->focusEnabled(getBoolKey(elem, "focusOnClick", true));
			game.Resources().addFocused(button);
		}
	}
}
