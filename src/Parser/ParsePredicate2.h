#pragma once

#include "Game/Predicate.h"
#include "Json/JsonParser.h"
#include <memory>

class Game;

namespace Parser2
{
	std::shared_ptr<Predicate> getPredicateObj(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
}
