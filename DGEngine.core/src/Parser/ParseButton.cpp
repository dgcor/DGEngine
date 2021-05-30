#include "ParseButton.h"
#include "BitmapButton.h"
#include "BitmapText.h"
#include <cassert>
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "StringButton.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Button> parseBitmapButton(Game& game, const Value& elem)
	{
		auto button = std::make_shared<BitmapButton>();

		if (elem.HasMember("texture"sv) == true)
		{
			auto tex = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (tex == nullptr)
			{
				return nullptr;
			}
			button->setTexture(*tex.get(), true);
		}
		else if (isValidString(elem, "texturePack"))
		{
			auto tex = game.Resources().getTexturePack(elem["texturePack"sv].GetStringView());
			if (tex == nullptr)
			{
				return nullptr;
			}
			TextureInfoVar tiVar;
			if (tex->get(getUIntKey(elem, "textureIndex"), tiVar) == false)
			{
				return nullptr;
			}
			button->setTexture(tiVar);
		}
		else
		{
			return nullptr;
		}

		if (elem.HasMember("textureRect"sv))
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

		return button;
	}

	std::shared_ptr<Button> parseStringButton(Game& game, const Value& elem)
	{
		auto drawableText = getDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return nullptr;
		}
		auto button = std::make_shared<StringButton>(std::move(drawableText));
		parseTextObj(game, elem, *button);
		return button;
	}

	std::shared_ptr<Button> parseButtonObj(Game& game, const Value& elem)
	{
		if (elem.HasMember("font"sv) == false)
		{
			return parseBitmapButton(game, elem);
		}
		else
		{
			return parseStringButton(game, elem);
		}
	}

	void parseButton(Game& game, const Value& elem,
		const parseButtonObjFuncPtr parseButtonObjFunc)
	{
		assert(parseButtonObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto button = parseButtonObjFunc(game, elem);
		if (button == nullptr)
		{
			return;
		}

		button->enable(getBoolKey(elem, "enable", true));
		button->setClickUp(getBoolKey(elem, "clickUp"));
		button->setCaptureInputEvents(getInputEventTypeKey(elem, "captureInputEvents"));

		if (elem.HasMember("onChange"sv))
		{
			button->setAction(str2int16("change"), getActionVal(game, elem["onChange"sv]));
		}
		if (elem.HasMember("onClick"sv))
		{
			button->setAction(str2int16("click"), getActionVal(game, elem["onClick"sv]));
		}
		if (elem.HasMember("onRightClick"sv))
		{
			button->setAction(str2int16("rightClick"), getActionVal(game, elem["onRightClick"sv]));
		}
		if (elem.HasMember("onDoubleClick"sv))
		{
			button->setAction(str2int16("doubleClick"), getActionVal(game, elem["onDoubleClick"sv]));
		}
		if (elem.HasMember("onClickDrag"sv))
		{
			button->setAction(str2int16("clickDrag"), getActionVal(game, elem["onClickDrag"sv]));
		}
		if (elem.HasMember("onClickIn"sv))
		{
			button->setAction(str2int16("clickIn"), getActionVal(game, elem["onClickIn"sv]));
		}
		if (elem.HasMember("onClickOut"sv))
		{
			button->setAction(str2int16("clickOut"), getActionVal(game, elem["onClickOut"sv]));
		}
		if (elem.HasMember("onFocus"sv))
		{
			button->setAction(str2int16("focus"), getActionVal(game, elem["onFocus"sv]));
		}
		if (elem.HasMember("onHoverEnter"sv))
		{
			button->setAction(str2int16("hoverEnter"), getActionVal(game, elem["onHoverEnter"sv]));
		}
		if (elem.HasMember("onHoverLeave"sv))
		{
			button->setAction(str2int16("hoverLeave"), getActionVal(game, elem["onHoverLeave"sv]));
		}
		if (isValidString(elem, "sound"))
		{
			button->setClickSound(game.Resources().getSoundBuffer(elem["sound"sv].GetStringView()));
		}
		if (isValidString(elem, "focusSound"))
		{
			button->setFocusSound(game.Resources().getSoundBuffer(elem["focusSound"sv].GetStringView()));
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
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

	void parseButton(Game& game, const Value& elem)
	{
		parseButton(game, elem, parseButtonObj);
	}
}
