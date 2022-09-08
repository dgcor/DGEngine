#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseFocusAdd(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseFocusClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseFocusMoveDown();

	std::shared_ptr<Action> parseFocusMoveUp();

	std::shared_ptr<Action> parseFocusRightClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseFocusSet(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseFocusUpdate(const rapidjson::Value& elem);
}
