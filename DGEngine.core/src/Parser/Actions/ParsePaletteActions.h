#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parsePaletteReplace(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePaletteShiftLeft(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePaletteShiftRight(const rapidjson::Value& elem);
}
