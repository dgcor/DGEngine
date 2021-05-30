#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Predicates/Predicate.h"

class Game;

namespace Parser
{
	std::shared_ptr<Predicate> getPredicateObj(Game& game, const rapidjson::Value& elem);
}
