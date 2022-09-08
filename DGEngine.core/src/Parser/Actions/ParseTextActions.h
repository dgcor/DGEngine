#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseTextAppendText(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseTextSetColor(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseTextSetFont(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseTextSetSpacing(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseTextSetText(const rapidjson::Value& elem);
}
