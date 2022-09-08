#include "ParseQuestActions.h"
#include "Game/Actions/ActQuest.h"
#include "Parser/Quest/ParseQuest.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser2::Actions
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseQuestAdd(Game& game, const Value& elem)
	{
		if (elem.HasMember("quest"sv) == true)
		{
			auto quest = parseQuestObj(game, elem["quest"sv]);
			if (isValidId(quest.Id()) == false)
			{
				return nullptr;
			}
			return std::make_shared<ActQuestAdd>(
				getStringViewKey(elem, "level"),
				std::move(quest));
		}
		return nullptr;
	}

	std::shared_ptr<Action> parseQuestDelete(const Value& elem)
	{
		return std::make_shared<ActQuestDelete>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "quest"));
	}

	std::shared_ptr<Action> parseQuestSetState(const Value& elem)
	{
		return std::make_shared<ActQuestSetState>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "quest"),
			getIntKey(elem, "state"));
	}
}
