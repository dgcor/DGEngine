#include "ParseCondition.h"
#include "Actions/ActCondition.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

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
		if (elem.HasMember("list"sv))
		{
			for (const auto& val : elem["list"sv])
			{
				list.push_back(getVariableVal(val));
			}
		}
		return std::make_shared<ActInListCondition>(
			getVarOrPredicateKey(game, elem, "param"),
			std::move(list),
			getActionKey(game, elem, "then"),
			getActionKey(game, elem, "else"));
	}

	void parseMultiIfCondition(Game& game, IfCondition& conditions, const Value& elem)
	{
		size_t step = 0;
		uint16_t conditionHash16 = str2int16("==");
		VarOrPredicate param1;
		VarOrPredicate param2;
		for (const auto& val : elem)
		{
			// step 0 - param 1 - can be a new condition array
			// step 1 - == != > >= < <= regex
			// step 2 - param 2 - adds condition ex: (param 1 > param2)
			// step 3 - and/or - ex: param1 == param2 and param3 == param4

			if (step == 1)
			{
				conditionHash16 = str2int16(getStringViewVal(val));
			}
			else if (step == 3)
			{
				switch (str2int16(getStringViewVal(val)))
				{
				default:
				case str2int16("and"):
					conditions.addCondition(IfCondition::ConditionOp::And);
					break;
				case str2int16("or"):
					conditions.addCondition(IfCondition::ConditionOp::Or);
					break;
				}
				step = 0;
				continue;
			}
			else
			{
				if (val.IsArray() == true)
				{
					conditions.addCondition(IfCondition::ConditionOp::LeftBracket);
					parseMultiIfCondition(game, conditions, val);
					conditions.addCondition(IfCondition::ConditionOp::RightBracket);
					step = 3;
					continue;
				}
				if (step == 0)
				{
					param1 = getVarOrPredicateVal(game, val);
				}
				else if (step == 2)
				{
					param2 = getVarOrPredicateVal(game, val);
					conditions.addCondition(conditionHash16, param1, param2);
					conditionHash16 = str2int16("==");
					param1 = {};
					param2 = {};
				}
			}
			step++;
		}
	}

	std::shared_ptr<Action> getMultiIfCondition(Game& game, const Value& elem)
	{
		if (isValidArray(elem, "params") == false)
		{
			return nullptr;
		}
		auto action = std::make_shared<ActMultiIfCondition>(
			getActionKey(game, elem, "then"),
			getActionKey(game, elem, "else"));

		parseMultiIfCondition(game, action->Conditions(), elem["params"sv]);
		return action;
	}

	std::shared_ptr<Action> getSwitchConditionHelper(
		Game& game, const Value& elem, const char* actionName,
		const std::vector<std::pair<Variable, std::shared_ptr<Action>>>& cases)
	{
		if (elem.HasMember(actionName) == true &&
			elem[actionName].IsObject() == false)
		{
			auto actionRef = getVariableKey(elem, actionName);
			for (const auto& caseObj : cases)
			{
				if (caseObj.first == actionRef)
				{
					return caseObj.second;
				}
			}
		}
		return getActionKey(game, elem, actionName);
	}

	std::shared_ptr<Action> getSwitchCondition(Game& game, const Value& elem)
	{
		std::vector<std::pair<Variable, std::shared_ptr<Action>>> cases;
		if (elem.HasMember("case"sv))
		{
			for (const auto& val : elem["case"sv])
			{
				if (val.IsObject() == true &&
					val.HasMember("value"sv) == true)
				{
					cases.push_back(
						std::make_pair(
							getVariableKey(val, "value"),
							getSwitchConditionHelper(game, val, "action", cases)
						)
					);
				}
			}
		}
		return std::make_shared<ActSwitchCondition>(
			getVarOrPredicateKey(game, elem, "param"),
			std::move(cases),
			getSwitchConditionHelper(game, elem, "default", cases));
	}
}
