#pragma once

#include "Game/Drawables/BindableText.h"
#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include <type_traits>

class Game;

namespace Parser
{
	void parseTextObj(Game& game, const rapidjson::Value& elem, Text& text);

	// parse "text"/"file" property of Text
	void parseTextDisplayText(const rapidjson::Value& elem, Text& text);

	template<class Txt = BindableText>
	std::unique_ptr<Txt> getTextObj(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<Text, Txt>);

		auto font = game.Resources().getFont(getStringKey(elem, "font"));
		if (holdsNullFont(font) == true)
		{
			return nullptr;
		}

		auto fontSize = getUIntKey(elem, "fontSize");
		auto text = std::make_unique<Txt>(font, fontSize);

		parseTextObj(game, elem, *text);

		return text;
	}

	void parseBindableTextObj(Game& game, const rapidjson::Value& elem, BindableText& text);

	template<class Txt = BindableText>
	std::unique_ptr<Txt> getBindableTextObj(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<Text, Txt>);

		auto text = getTextObj<Txt>(game, elem);
		if (text == nullptr)
		{
			return nullptr;
		}
		parseBindableTextObj(game, elem, *text);
		return text;
	}

	void parseText(Game& game, const rapidjson::Value& elem);
}
