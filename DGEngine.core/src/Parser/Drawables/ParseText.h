#pragma once

#include "Game/Drawables/BindableText.h"
#include "Json/JsonParser.h"
#include <memory>

class DrawableText;
class Game;

namespace Parser
{
	void parseDrawableTextObj(Game& game, const rapidjson::Value& elem, DrawableText& text);

	// parse "text"/"file" property of DrawablText
	void parseDrawableTextDisplayText(const rapidjson::Value& elem, DrawableText& text);

	std::unique_ptr<DrawableText> getDrawableTextObj(Game& game, const rapidjson::Value& elem);

	void parseBindableTextObj(Game& game, const rapidjson::Value& elem, BindableText& text);

	template<class Txt = BindableText>
	std::unique_ptr<Txt> getBindableTextObj(Game& game, const rapidjson::Value& elem)
	{
		auto drawableText = getDrawableTextObj(game, elem);
		if (drawableText == nullptr)
		{
			return nullptr;
		}
		auto text = std::make_unique<Txt>(std::move(drawableText));
		parseBindableTextObj(game, elem, *text);
		return text;
	}

	void parseText(Game& game, const rapidjson::Value& elem);
}
