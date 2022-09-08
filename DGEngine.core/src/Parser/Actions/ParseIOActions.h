#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseDirCopy(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDirCreate(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseFileCopy(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseIODelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseIODeleteAll(const rapidjson::Value& elem);
}
