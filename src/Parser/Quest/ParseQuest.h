#pragma once

#include "Game/Quest/Quest.h"
#include "Json/JsonParser.h"

class Game;

namespace Parser
{
	Quest parseQuestObj(Game& game, const rapidjson::Value& elem);
	void parseQuest(Game& game, const rapidjson::Value& elem);
}
