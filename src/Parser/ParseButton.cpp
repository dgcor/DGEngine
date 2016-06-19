#include "ParseButton.h"
#include "BitmapButton.h"
#include "BitmapText.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "ParseUtils.h"
#include "StringButton.h"
#include "StringText.h"

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
			button->setTextureRect(getIntRect(elem, "textureRect", rect));
		}
		button->setResizable(getBool(elem, "resizable"));

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

		std::shared_ptr<Button> button;

		if (elem.HasMember("texture"))
		{
			button = parseBitmapButton(game, elem);
			if (button == nullptr)
			{
				return;
			}
			auto anchor = getAnchor(elem, "anchor");
			button->setAnchor(anchor);
			auto pos = getVector2f<sf::Vector2f>(elem, "position");
			auto size = button->Size();
			if (getBool(elem, "relativeCoords", true) == true)
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
			button->Visible(getBool(elem, "visible", true));

			button->setColor(getColor(elem, "color", sf::Color::White));
		}
		else
		{
			button = parseStringButton(game, elem);
			if (button == nullptr)
			{
				return;
			}
		}

		button->enable(getBool(elem, "enable", true));
		button->setClickUp(getBool(elem, "clickUp"));

		if (elem.HasMember("onClick"))
		{
			button->setClickAction(parseAction(game, elem["onClick"]));
		}

		if (elem.HasMember("onDoubleClick"))
		{
			button->setDoubleClickAction(parseAction(game, elem["onDoubleClick"]));
		}

		if (elem.HasMember("onToggle"))
		{
			button->setToggleAction(parseAction(game, elem["onToggle"]));
		}

		if (elem.HasMember("onClickDrag"))
		{
			button->setClickDragAction(parseAction(game, elem["onClickDrag"]));
		}

		if (elem.HasMember("onClickIn"))
		{
			button->setClickInAction(parseAction(game, elem["onClickIn"]));
		}

		if (elem.HasMember("onClickOut"))
		{
			button->setClickOutAction(parseAction(game, elem["onClickOut"]));
		}

		if (elem.HasMember("onFocus"))
		{
			button->setFocusAction(parseAction(game, elem["onFocus"]));
		}

		if (elem.HasMember("onHoverEnter"))
		{
			button->setHoverEnterAction(parseAction(game, elem["onHoverEnter"]));
		}

		if (elem.HasMember("onHoverLeave"))
		{
			button->setHoverLeaveAction(parseAction(game, elem["onHoverLeave"]));
		}

		if (elem.HasMember("sound"))
		{
			button->setClickSound(game.Resources().getSound(elem["sound"].GetString()));
		}

		if (elem.HasMember("focusSound"))
		{
			button->setFocusSound(game.Resources().getSound(elem["focusSound"].GetString()));
		}

		game.Resources().addDrawable(elem["id"].GetString(), button);

		if (getBool(elem, "focus") == true)
		{
			button->focusEnabled(getBool(elem, "focusOnClick", true));
			game.Resources().addFocused(button);
		}
	}
}
