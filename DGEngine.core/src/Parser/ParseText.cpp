#include "ParseText.h"
#include "BindableText.h"
#include "BitmapText.h"
#include <cassert>
#include "DrawableText.h"
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "ParseAction.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::unique_ptr<DrawableText> getDrawableTextObj(Game& game, const Value& elem)
	{
		auto font = game.Resources().getFont(getStringKey(elem, "font"));
		if (holdsNullFont(font) == true)
		{
			return nullptr;
		}

		std::unique_ptr<DrawableText> text;

		if (holdsFreeTypeFont(font) == true)
		{
			auto fontSize = getUIntKey(elem, "fontSize", 12);
			text = std::make_unique<StringText>(std::get<std::shared_ptr<FreeTypeFont>>(font), fontSize);
		}
		else
		{
			text = std::make_unique<BitmapText>(std::get<std::shared_ptr<BitmapFont>>(font));
		}

		text->setHorizontalSpaceOffset(getIntKey(elem, "horizontalSpaceOffset"));
		text->setVerticalSpaceOffset(getIntKey(elem, "verticalSpaceOffset"));
		text->setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringViewKey(elem, "horizontalAlign")));
		text->setVerticalAlign(GameUtils::getVerticalAlignment(getStringViewKey(elem, "verticalAlign"), VerticalAlign::Bottom));

		std::string displayText;

		if (elem.HasMember("text"sv) == true)
		{
			displayText = getStringVal(elem["text"sv]);
		}
		else if (elem.HasMember("file"sv) == true)
		{
			displayText = FileUtils::readText(getStringCharVal(elem["file"sv]));
		}

		if (elem.HasMember("splitText"sv) == true)
		{
			auto split = getUIntVal(elem["splitText"sv]);
			if (split > 0)
			{
				displayText = Utils::splitInLines(displayText, split);
			}
		}

		text->setColor(getColorKey(elem, "color", sf::Color::White));
		text->setText(displayText);

		auto anchor = getAnchorKey(elem, "anchor");
		text->setAnchor(anchor);
		auto size = text->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		text->Position(pos);
		text->Visible(getBoolKey(elem, "visible", true));

		return text;
	}

	std::unique_ptr<BindableText> getTextObj(Game& game, const Value& elem)
	{
		auto drawableText = getDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return nullptr;
		}
		auto text = std::make_unique<BindableText>(std::move(drawableText));
		parseTextObj(game, elem, *text);
		return text;
	}

	void parseTextObj(Game& game, const Value& elem, BindableText& text)
	{
		auto hasBinding = elem.HasMember("binding"sv) == true;
		if (hasBinding == true)
		{
			text.setBinding(getStringVectorKey(elem, "binding"));
			text.setFormat(getStringViewKey(elem, "format", "[1]"));
			text.setBindingFlags(getBindingFlagsKey(elem, "bindingFlags"));
		}
		if (elem.HasMember("onChange"sv))
		{
			text.setAction(str2int16("change"), getActionVal(game, elem["onChange"sv]));
		}
		if (hasBinding == true)
		{
			text.updateText(game);
		}
	}

	void parseText(Game& game, const Value& elem,
		const getTextObjFuncPtr getTextObjFunc)
	{
		assert(getTextObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		std::shared_ptr<Text> text(getTextObj(game, elem));
		if (text == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(text);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, text, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}

	void parseText(Game& game, const Value& elem)
	{
		parseText(game, elem, getTextObj);
	}
}
