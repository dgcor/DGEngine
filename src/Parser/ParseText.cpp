#include "ParseText.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "GameUtils.h"
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

		std::string displayText;

		if (elem.HasMember("text") == true)
		{
			displayText = getStringVal(elem["text"]);
		}
		else if (elem.HasMember("file") == true)
		{
			displayText = FileUtils::readText(getStringCharVal(elem["file"]));
		}

		std::unique_ptr<DrawableText> text;

		auto horizSpaceOffset = getIntKey(elem, "horizontalSpaceOffset");
		auto vertSpaceOffset = getIntKey(elem, "verticalSpaceOffset");

		if (holdsFreeTypeFont(font) == true)
		{
			auto fontSize = getUIntKey(elem, "fontSize", 12);
			text = std::make_unique<StringText>(displayText,
				*std::get<std::shared_ptr<FreeTypeFont>>(font), fontSize);

			text->setHorizontalSpaceOffset(horizSpaceOffset);
			text->setVerticalSpaceOffset(vertSpaceOffset);
		}
		else
		{
			text = std::make_unique<BitmapText>(displayText,
				std::get<std::shared_ptr<BitmapFont>>(font), horizSpaceOffset, vertSpaceOffset);
		}

		text->setColor(getColorKey(elem, "color", sf::Color::White));
		text->setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringKey(elem, "horizontalAlign")));
		text->setVerticalAlign(GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign"), VerticalAlign::Bottom));

		auto anchor = getAnchorKey(elem, "anchor");
		text->setAnchor(anchor);
		auto size = text->Size();
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		text->Position(pos);
		text->Visible(getBoolKey(elem, "visible", true));

		return text;
	}

	std::unique_ptr<Text> parseText2Obj(Game& game, const Value& elem)
	{
		auto text = std::make_unique<Text>();
		if (parseText2Obj(game, elem, *text) == false)
		{
			return nullptr;
		}
		return text;
	}

	bool parseText2Obj(Game& game, const Value& elem, Text& text)
	{
		auto drawableText = parseDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return false;
		}
		text.setText(std::move(drawableText));

		auto hasBinding = elem.HasMember("binding") == true;
		if (hasBinding == true)
		{
			text.setBinding(getStringVectorKey(elem, "binding"));
		}
		text.setFormat(getStringViewKey(elem, "format", "[1]"));

		if (elem.HasMember("onChange"))
		{
			text.setAction(str2int16("change"), parseAction(game, elem["onChange"]));
		}
		if (hasBinding == true)
		{
			text.update(game);
		}
		return true;
	}

	void parseText(Game& game, const Value& elem)
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
		std::shared_ptr<Text> text(parseText2Obj(game, elem));
		if (text == nullptr)
		{
			return;
		}
		game.Resources().addDrawable(id, text, getStringViewKey(elem, "resource"));
	}
}
