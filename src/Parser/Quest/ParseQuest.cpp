#include "ParseQuest.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	Quest parseQuestObj(Game& game, const Value& elem)
	{
		return Quest(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "name"),
			getIntKey(elem, "state")
		);
	}

	void parseQuest(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
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

		level->addQuest(std::move(quest));
	}
}
