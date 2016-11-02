#include "ParseQuest.h"
#include "Parser/Utils/ParseUtils.h"

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
		if (quest.Id().empty() == true ||
			isValidId(quest.Id()) == false)
		{
			return;
		}

		level->addQuest(quest);
	}
}
