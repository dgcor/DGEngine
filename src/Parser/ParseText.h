#pragma once

#include "Game.h"
#include "DrawableText.h"
#include "Json/JsonParser.h"
#include "Text2.h"

namespace Parser
{
	std::unique_ptr<DrawableText> parseDrawableTextObj(Game& game, const rapidjson::Value& elem);
	std::unique_ptr<Text2> parseText2Obj(Game& game, const rapidjson::Value& elem);
	void parseText(Game& game, const rapidjson::Value& elem);
}
