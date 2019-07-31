#pragma once

#include "BindableText.h"
#include "DrawableText.h"
#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	std::unique_ptr<DrawableText> parseDrawableTextObj(Game& game, const rapidjson::Value& elem);
	void parseTextObj(Game& game, const rapidjson::Value& elem, BindableText& text);
	void parseText(Game& game, const rapidjson::Value& elem);
}
