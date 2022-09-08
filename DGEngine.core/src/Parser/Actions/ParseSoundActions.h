#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseSoundLoadPlay(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseSoundPlay(const rapidjson::Value& elem);
}
