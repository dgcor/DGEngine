#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseCursorCenterOnDrawable(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorEnableOutline(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorPop(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorSetColor(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorSetOutline(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorSetPalette(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorSetPosition(const rapidjson::Value& elem);
	
	std::shared_ptr<Action> parseCursorShow(const rapidjson::Value& elem);
}
