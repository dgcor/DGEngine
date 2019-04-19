#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Predicates/Predicate.h"

class Game;

namespace Parser
{
	std::shared_ptr<Predicate> parsePredicateObj(Game& game, const rapidjson::Value& elem);
}
