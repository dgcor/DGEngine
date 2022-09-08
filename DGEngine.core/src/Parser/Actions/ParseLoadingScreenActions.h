#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseLoadingScreenClear();

	std::shared_ptr<Action> parseLoadingScreenSetProgress(const rapidjson::Value& elem);
}
