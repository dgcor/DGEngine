#include "ParseText.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/GameUtils.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseTextObj(Game& game, const Value& elem, Text& text)
	{
		text.setHorizontalSpaceOffset(getIntKey(elem, "horizontalSpaceOffset"));
		text.setVerticalSpaceOffset(getIntKey(elem, "verticalSpaceOffset"));
		text.setHorizontalAlign(GameUtils::getHorizontalAlignment(getStringViewKey(elem, "horizontalAlign")));
		text.setVerticalAlign(GameUtils::getVerticalAlignment(getStringViewKey(elem, "verticalAlign"), VerticalAlign::Bottom));

		text.setColor(getColorKey(elem, "color", sf::Color::White));

		parseTextDisplayText(elem, text);

		parseDrawableProperties(game, elem, text);
	}

	void parseTextDisplayText(const Value& elem, Text& text)
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

		if (elem.HasMember("wordWrap"sv) == true)
		{
			auto maxLength = getUIntVal(elem["wordWrap"sv]);
			if (maxLength > 0)
			{
				displayText = Utils::wordWrap(displayText, maxLength);
			}
		}

		text.setText(displayText);
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
