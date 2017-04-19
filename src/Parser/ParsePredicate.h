#pragma once

#include "Game.h"
#include <memory>
#include "Predicates/Predicate.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::shared_ptr<Predicate> parsePredicateObj(Game& game, const rapidjson::Value& elem);
}
