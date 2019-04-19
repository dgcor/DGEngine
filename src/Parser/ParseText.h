#pragma once

#include "DrawableText.h"
#include "Json/JsonParser.h"
#include "Text.h"

class Game;

namespace Parser
{
	std::unique_ptr<DrawableText> parseDrawableTextObj(Game& game, const rapidjson::Value& elem);
	std::unique_ptr<Text> parseText2Obj(Game& game, const rapidjson::Value& elem);
	bool parseText2Obj(Game& game, const rapidjson::Value& elem, Text& text);
	void parseText(Game& game, const rapidjson::Value& elem);
}
