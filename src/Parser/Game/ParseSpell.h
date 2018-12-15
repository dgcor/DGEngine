#pragma once

#include "Game.h"
#include "Game/Spell.h"
#include "Json/JsonParser.h"

namespace Parser
{
	void parseSpell(Game& game, const rapidjson::Value& elem);
}
