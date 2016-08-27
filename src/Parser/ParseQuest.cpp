#include "ParseQuest.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	Quest parseQuestObj(Game& game, const rapidjson::Value& elem)
	{
		return Quest(getStringKey(elem, "id"),
			getStringKey(elem, "name"), getIntKey(elem, "state"));
	}

	void parseQuest(Game& game, const rapidjson::Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto quest = parseQuestObj(game, elem);
		if (quest.Id().empty() == true)
		{
			return;
		}

		level->addQuest(quest);
	}
}
