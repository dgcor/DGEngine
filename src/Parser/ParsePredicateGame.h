#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include "Predicates/Predicate.h"

class Game;

namespace Parser
{
	std::shared_ptr<Predicate> getPredicateGameObj(Game& game,
		uint16_t nameHash16, const rapidjson::Value& elem);
}
