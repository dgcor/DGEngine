#include "ParseCondition.h"
#include "Actions/ActCondition.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Action> getIfCondition(unsigned conditionHash,
		Game& game, const Value& elem)
	{
		std::shared_ptr<Action> actThen;
		std::shared_ptr<Action> actElse;
		if (elem.HasMember("then"))
		{
			actThen = parseAction(game, elem["then"]);
		}
		if (elem.HasMember("else"))
		{
			actElse = parseAction(game, elem["else"]);
		}
		return std::make_shared<ActIfCondition>(
			conditionHash,
			getVariableKey(elem, "param1"),
			getVariableKey(elem, "param2"),
			actThen,
			actElse);
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
		std::shared_ptr<Action> actThen;
		std::shared_ptr<Action> actElse;
		if (elem.HasMember("then"))
		{
			actThen = parseAction(game, elem["then"]);
		}
		if (elem.HasMember("else"))
		{
			actElse = parseAction(game, elem["else"]);
		}
		return std::make_shared<ActInListCondition>(
			getVariableKey(elem, "param"),
			list,
			actThen,
			actElse);
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
					if (val.HasMember("action") == true)
					{
						cases.push_back(
							std::make_pair(getVariableKey(val, "value"),
								parseAction(game, val["action"]))
						);
					}
					else
					{
						cases.push_back(std::make_pair(getVariableKey(val, "value"), nullptr));
					}
				}
			}
		}
		std::shared_ptr<Action> actDefault;
		if (elem.HasMember("default"))
		{
			actDefault = parseAction(game, elem["default"]);
		}
		return std::make_shared<ActSwitchCondition>(
			getVariableKey(elem, "param"),
			cases,
			actDefault);
	}
}
