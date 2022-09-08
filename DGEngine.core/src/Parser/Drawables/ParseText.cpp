#include "ParseText.h"
#include "Game/Drawables/BitmapText.h"
#include "Game/Drawables/DrawableText.h"
#include "Game/Drawables/StringText.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/GameUtils.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseDrawableTextObj(Game& game, const Value& elem, DrawableText& text)
	{
		text.setHorizontalSpaceOffset(getIntKey(elem, "horizontalSpaceOffset"));
		text.setVerticalSpaceOffset(getIntKey(elem, "verticalSpaceOffset"));
		text.setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringViewKey(elem, "horizontalAlign")));
		text.setVerticalAlign(GameUtils::getVerticalAlignment(getStringViewKey(elem, "verticalAlign"), VerticalAlign::Bottom));

		text.setColor(getColorKey(elem, "color", sf::Color::White));

		parseDrawableTextDisplayText(elem, text);

		parseDrawableProperties(game, elem, text);
	}

	void parseDrawableTextDisplayText(const Value& elem, DrawableText& text)
	{
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

		text.setText(displayText);
	}

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
			auto fontSize = getUIntKey(elem, "fontSize");
			text = std::make_unique<StringText>(std::get<std::shared_ptr<FreeTypeFont>>(font), fontSize);
		}
		else
		{
			text = std::make_unique<BitmapText>(std::get<std::shared_ptr<BitmapFont>>(font));
		}

		parseDrawableTextObj(game, elem, *text);

		return text;
	}

	void parseBindableTextObj(Game& game, const Value& elem, BindableText& text)
	{
		auto hasBinding = elem.HasMember("binding"sv) == true;
		if (hasBinding == true)
		{
			text.setBinding(getStringVectorKey(elem, "binding"));
			if (getBoolKey(elem, "bindToFormat", true) == true)
			{
				text.setFormat(getStringViewKey(elem, "format", "[1]"));
			}
			else
			{
				text.setFormat(text.getText());
			}
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

	static std::shared_ptr<UIObject> parseTextFunc(Game& game, const Value& elem)
	{
		return getBindableTextObj(game, elem);
	}

	void parseText(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseTextFunc);
	}
}
