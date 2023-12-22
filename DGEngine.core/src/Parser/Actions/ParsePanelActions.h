#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parsePanelAddDrawable(const rapidjson::Value& elem);
}
