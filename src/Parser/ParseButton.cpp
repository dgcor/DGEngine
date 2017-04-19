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
		button->setCaptureScrollEvent(getBoolKey(elem, "captureScrollEvent"));

		return button;
	}

	std::shared_ptr<Button> parseStringButton(Game& game, const Value& elem)
	{
		auto text = parseText2Obj(game, elem);
		if (text == nullptr)
		{
			return nullptr;
		}
		auto button = std::make_shared<StringButton>();
		button->setText(std::move(text));
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

		if (elem.HasMember("texture"))
		{
			button = parseBitmapButton(game, elem);
			if (button == nullptr)
			{
				return;
			}
			auto anchor = getAnchorKey(elem, "anchor");
			button->setAnchor(anchor);
			auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
			auto size = button->Size();
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
		else
		{
			button = parseStringButton(game, elem);
			if (button == nullptr)
			{
				return;
			}
		}

		button->enable(getBoolKey(elem, "enable", true));
		button->setClickUp(getBoolKey(elem, "clickUp"));

		if (elem.HasMember("onClick"))
		{
			button->setAction(str2int16("click"), parseAction(game, elem["onClick"]));
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
			button->setClickSound(game.Resources().getSound(elem["sound"].GetString()));
		}
		if (elem.HasMember("focusSound"))
		{
			button->setFocusSound(game.Resources().getSound(elem["focusSound"].GetString()));
		}

		game.Resources().addDrawable(id, button);

		if (getBoolKey(elem, "focus") == true)
		{
			button->focusEnabled(getBoolKey(elem, "focusOnClick", true));
			game.Resources().addFocused(button);
		}
	}
}
