#include "ParseCondition.h"
#include "Actions/ActCondition.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Action> getIfCondition(uint16_t conditionHash16,
		Game& game, const Value& elem)
	{
		return std::make_shared<ActIfCondition>(
			conditionHash16,
			getVarOrPredicateKey(game, elem, "param1"),
			getVarOrPredicateKey(game, elem, "param2"),
			getActionKey(game, elem, "then"),
			getActionKey(game, elem, "else"));
	}

	std::shared_ptr<Action> getInListCondition(Game& game, const Value& elem)
	{
		std::vector<Variable> list;
		if (elem.HasMember("list"))
		{
			for (const auto& val : elem["list"])
			{
				list.push_back(getVariableVal(val));
			}
		}
		return std::make_shared<ActInListCondition>(
			getVarOrPredicateKey(game, elem, "param"),
			list,
			getActionKey(game, elem, "then"),
			getActionKey(game, elem, "else"));
	}

	std::shared_ptr<Action> getSwitchCondition(Game& game, const Value& elem)
	{
		std::vector<std::pair<Variable, std::shared_ptr<Action>>> cases;
		if (elem.HasMember("case"))
		{
			for (const auto& val : elem["case"])
			{
				if (val.IsObject() == true &&
					val.HasMember("value") == true)
				{
					cases.push_back(
						std::make_pair(getVariableKey(val, "value"),
							getActionKey(game, val, "action"))
					);
				}
			}
		}
		return std::make_shared<ActSwitchCondition>(
			getVarOrPredicateKey(game, elem, "param"),
			cases,
			getActionKey(game, elem, "default"));
	}
}
