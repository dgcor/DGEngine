#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Predicates/Predicate.h"

class Game;

namespace Parser2
{
	std::shared_ptr<Predicate> getPredicateObj(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
}
