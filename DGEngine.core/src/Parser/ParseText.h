#pragma once

#include "Json/JsonParser.h"
#include <memory>

class BindableText;
class DrawableText;
class Game;

namespace Parser
{
	std::unique_ptr<DrawableText> getDrawableTextObj(Game& game, const rapidjson::Value& elem);

	std::unique_ptr<BindableText> getTextObj(Game& game, const rapidjson::Value& elem);

	void parseTextObj(Game& game, const rapidjson::Value& elem, BindableText& text);

	typedef decltype(&getTextObj) getTextObjFuncPtr;

	void parseText(Game& game, const rapidjson::Value& elem,
		const getTextObjFuncPtr getTextObjFunc);

	void parseText(Game& game, const rapidjson::Value& elem);
}
