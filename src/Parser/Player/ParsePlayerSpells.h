#pragma once

#include "Json/JsonParser.h"

class Level;
class Player;

namespace Parser
{
	void parsePlayerSpells(Player& player, Level& level, const rapidjson::Value& elem);
}
