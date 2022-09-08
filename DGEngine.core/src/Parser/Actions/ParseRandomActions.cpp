#include "ParseRandomActions.h"
#include "Game/Actions/ActRandom.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseRandom(Game& game, const Value& elem)
	{
		return std::make_shared<ActRandom>(
			getFloatKey(elem, "percentage", 0.5),
			getActionKey(game, elem, "action1"),
			getActionKey(game, elem, "action2"));
	}

	std::shared_ptr<Action> parseRandomList(Game& game, const Value& elem)
	{
		auto actionList = std::make_shared<ActRandomList>();
		bool hasActions = false;
		if (elem.HasMember("actions"sv) == true &&
			elem["actions"sv].IsArray() == true)
		{
			for (const auto& val : elem["actions"sv])
			{
				auto action = getActionVal(game, val);
				if (action != nullptr)
				{
					actionList->add(action);
					hasActions = true;
				}
			}
		}
		if (hasActions == false)
		{
			return nullptr;
		}
		return actionList;
	}
}
