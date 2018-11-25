#pragma once

#include "Game.h"
#include "Game/Spell.h"
#include "Json/JsonParser.h"

namespace Parser
{
	std::unique_ptr<Spell> parseSpellObj(Game& game,
		Level& level, const rapidjson::Value& elem);
}
