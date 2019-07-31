#include "ParseText.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Panel.h"
#include "ParseAction.h"
#include "StringText.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::unique_ptr<DrawableText> parseDrawableTextObj(Game& game, const Value& elem)
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
		text->setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringKey(elem, "horizontalAlign")));
		text->setVerticalAlign(GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign"), VerticalAlign::Bottom));

		std::string displayText;

		if (elem.HasMember("text") == true)
		{
			displayText = getStringVal(elem["text"]);
		}
		else if (elem.HasMember("file") == true)
		{
			displayText = FileUtils::readText(getStringCharVal(elem["file"]));
		}

		if (elem.HasMember("splitText") == true)
		{
			auto split = getUIntVal(elem["splitText"]);
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

	std::unique_ptr<BindableText> parseTextObj(Game& game, const Value& elem)
	{
		auto drawableText = parseDrawableTextObj(game, elem);
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
		auto hasBinding = elem.HasMember("binding") == true;
		if (hasBinding == true)
		{
			text.setBinding(getStringVectorKey(elem, "binding"));
			text.setFormat(getStringViewKey(elem, "format", "[1]"));
		}
		if (elem.HasMember("onChange"))
		{
			text.setAction(str2int16("change"), parseAction(game, elem["onChange"]));
		}
		if (hasBinding == true)
		{
			text.update(game);
		}
	}

	void parseText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"].GetStringStr();
		if (isValidId(id) == false)
		{
			return;
		}
		std::shared_ptr<Text> text(parseTextObj(game, elem));
		if (text == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			std::string panelId = getStringVal(elem["panel"]);
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
}
