#include "ParseButton.h"
#include "BitmapButton.h"
#include "BitmapText.h"
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
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
		if (isValidString(elem, "texture") == false &&
			isValidString(elem, "texturePack") == false)
		{
			return nullptr;
		}

		auto button = std::make_shared<BitmapButton>();

		if (elem.HasMember("texture") == true)
		{
			auto tex = game.Resources().getTexture(getStringVal(elem["texture"]));
			if (tex == nullptr)
			{
				return nullptr;
			}

			button->setTexture(*tex.get(), true);
		}
		else
		{
			auto tex = game.Resources().getTexturePack(getStringVal(elem["texturePack"]));
			if (tex == nullptr)
			{
				return nullptr;
			}
			TextureInfo ti;
			if (tex->get(getUIntKey(elem, "textureIndex"), ti) == false)
			{
				return nullptr;
			}
			button->setTexture(ti, true);
		}

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, game.DrawRegionSize().x, game.DrawRegionSize().y);
			button->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}
		button->Resizable(getBoolKey(elem, "resizable"));
		button->Stretch(getBoolKey(elem, "stretch"));

		auto outline = getColorKey(elem, "outline", sf::Color::Transparent);
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", sf::Color::Transparent);
		button->setOutline(outline, outlineIgnore);
		button->setOutlineEnabled(getBoolKey(elem, "enableOutline"));

		return button;
	}

	std::shared_ptr<Button> parseStringButton(Game& game, const Value& elem)
	{
		auto drawableText = parseDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return nullptr;
		}
		auto button = std::make_shared<StringButton>(std::move(drawableText));
		parseTextObj(game, elem, *button);
		return button;
	}

	void parseButton(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetStringStr());
		if (isValidId(id) == false)
		{
			return;
		}

		std::shared_ptr<Button> button;

		if (elem.HasMember("texture") == true ||
			elem.HasMember("texturePack") == true)
		{
			button = parseBitmapButton(game, elem);
			if (button != nullptr)
			{
				auto anchor = getAnchorKey(elem, "anchor");
				button->setAnchor(anchor);
				auto size = button->Size();
				auto pos = getPositionKey(elem, "position", size, game.RefSize());
				if (getBoolKey(elem, "relativeCoords", true) == true &&
					game.RefSize() != game.DrawRegionSize())
				{
					GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
				}
				button->Position(pos);
				auto bitmapBtn = static_cast<BitmapButton*>(button.get());
				bitmapBtn->Size(size);
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
		button->setCaptureInputEvents(getInputEventTypeKey(elem, "captureInputEvents"));

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
		if (isValidString(elem, "sound"))
		{
			button->setClickSound(game.Resources().getSoundBuffer(elem["sound"].GetStringStr()));
		}
		if (isValidString(elem, "focusSound"))
		{
			button->setFocusSound(game.Resources().getSoundBuffer(elem["focusSound"].GetStringStr()));
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			std::string panelId = getStringVal(elem["panel"]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(button);
				manageObjDrawing = false;
			}
		}

		auto resource = getStringViewKey(elem, "resource");

		game.Resources().addDrawable(
			id, button, manageObjDrawing, resource
		);

		if (getBoolKey(elem, "focus") == true)
		{
			button->focusEnabled(getBoolKey(elem, "focusOnClick", true));
			game.Resources().addFocused(button, resource);
		}
	}
}
