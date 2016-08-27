#include "ParseText.h"
#include "BitmapText.h"
#include "FileUtils.h"
#include "GameUtils.h"
#include "ParseUtils.h"
#include "StringText.h"

namespace Parser
{
	using namespace rapidjson;

	std::unique_ptr<DrawableText> parseDrawableTextObj(Game& game, const rapidjson::Value& elem)
	{
		std::string displayText;

		if (elem.HasMember("text"))
		{
			displayText = getStringVal(elem["text"]);
		}
		else if (elem.HasMember("file"))
		{
			displayText = FileUtils::readText(getStringCharVal(elem["file"]));
		}

		if (elem.HasMember("font"))
		{
			auto font = game.Resources().getFont(elem["font"].GetString());
			if (font == nullptr)
			{
				return nullptr;
			}

			auto size = getUIntKey(elem, "fontSize", 12);
			auto text = std::make_unique<StringText>(displayText, *font, size);
			text->setColor(getColorKey(elem, "color", sf::Color::White));
			text->setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringKey(elem, "horizontalAlign")));
			text->setVerticalAlign(GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign")));

			auto anchor = getAnchorKey(elem, "anchor");
			text->setAnchor(anchor);
			auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
			if (getBoolKey(elem, "relativeCoords", true) == true)
			{
				auto size = text->Size();
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
				if (game.StretchToFit() == false)
				{
					GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
				}
			}
			text->Position(pos);
			text->Visible(getBoolKey(elem, "visible", true));

			return std::move(text);
		}
		else if (elem.HasMember("bitmapFont"))
		{
			auto font = game.Resources().getBitmapFont(elem["bitmapFont"].GetString());
			if (font == nullptr)
			{
				return nullptr;
			}

			auto horizSpaceOffset = getIntKey(elem, "horizontalSpaceOffset");
			auto vertSpaceOffset = getIntKey(elem, "verticalSpaceOffset");

			auto text = std::make_unique<BitmapText>(displayText, font, horizSpaceOffset, vertSpaceOffset);
			text->setColor(getColorKey(elem, "color", sf::Color::White));
			text->setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringKey(elem, "horizontalAlign")));
			text->setVerticalAlign(GameUtils::getVerticalAlignment(getStringKey(elem, "verticalAlign")));

			auto anchor = getAnchorKey(elem, "anchor");
			text->setAnchor(anchor);
			auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
			if (getBoolKey(elem, "relativeCoords", true) == true)
			{
				auto size = text->Size();
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
				if (game.StretchToFit() == false)
				{
					GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
				}
			}
			text->Position(pos);
			text->Visible(getBoolKey(elem, "visible", true));

			return std::move(text);
		}
		return nullptr;
	}

	std::unique_ptr<Text2> parseText2Obj(Game& game, const rapidjson::Value& elem)
	{
		auto drawableText = parseDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return nullptr;
		}

		auto text = std::make_unique<Text2>(std::move(drawableText));
		auto hasBinding = elem.HasMember("binding") == true;
		if (hasBinding == true)
		{
			text->setBinding(getStringVectorKey(elem, "binding"));
		}
		text->setFormat(getStringKey(elem, "format", "[1]"));
		if (hasBinding == true)
		{
			text->update(game);
		}
		return text;
	}

	void parseText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		std::shared_ptr<Text2> text(std::move(parseText2Obj(game, elem)));
		if (text == nullptr)
		{
			return;
		}

		game.Resources().addDrawable(elem["id"].GetString(), text);
	}
}
