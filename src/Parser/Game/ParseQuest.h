#pragma once

#include "Game.h"
#include "Game/Quest.h"
#include "Json/JsonParser.h"

namespace Parser
{
	Quest parseQuestObj(Game& game, const rapidjson::Value& elem);
	void parseQuest(Game& game, const rapidjson::Value& elem);
}
