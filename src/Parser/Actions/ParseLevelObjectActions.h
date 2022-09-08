#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2::Actions
{
	std::shared_ptr<Action> parseLevelObjDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelObjDeleteByClass(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelObjExecuteAction(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelObjSetOutline(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelObjSetPalette(const rapidjson::Value& elem);
}
