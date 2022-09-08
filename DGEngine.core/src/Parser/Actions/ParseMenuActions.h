#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseMenuAppendText(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuClickVisible(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuMoveScrollbar(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuRightClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuRightClickVisible(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuSetColor(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuSetFont(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuSetIndex(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMenuSetText(const rapidjson::Value& elem);
}
