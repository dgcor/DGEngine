#pragma once

#include "Game/Predicate.h"
#include "Json/JsonParser.h"
#include <memory>

class Game;

namespace Parser
{
	std::shared_ptr<Predicate> getPredicateObj(Game& game, const rapidjson::Value& elem);
}
