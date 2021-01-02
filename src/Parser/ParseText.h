#pragma once

#include "Json/JsonParser.h"
#include <memory>

class BindableText;
class DrawableText;
class Game;

namespace Parser
{
	std::unique_ptr<DrawableText> getDrawableTextObj(Game& game, const rapidjson::Value& elem);
	void parseTextObj(Game& game, const rapidjson::Value& elem, BindableText& text);
	void parseText(Game& game, const rapidjson::Value& elem);
}
