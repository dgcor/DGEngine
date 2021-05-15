#include "IfCondition.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Regex.h"
#include "Utils/Utils.h"

Variable IfCondition::getVariable(const Game& game, const VarOrPredicate& varOrPred)
{
	if (holdsVariable(varOrPred) == true)
	{
		const auto& var = std::get<Variable>(varOrPred);
		if (std::holds_alternative<std::string>(var) == true)
		{
			const auto& str = std::get<std::string>(var);
			Variable var2;
			if (str.empty() == false && str[0] == '#')
			{
				std::string_view strNoShebang{ str.data() + 1, str.size() - 1 };
				auto str2 = GameUtils::replaceStringWithVarOrProp(strNoShebang, game, '!');
				if (game.getVarOrProp(str2, var2) == true)
				{
					return var2;
				}
				return str2;
			}
			if (game.getVarOrProp(str, var2) == true)
			{
				return var2;
			}
			return str;
		}
		return var;
	}
	else
	{
		auto predicate = std::get<std::shared_ptr<Predicate>>(varOrPred).get();
		if (predicate != nullptr)
		{
			return predicate->getResult(game);
		}
		return {};
	}
}

bool IfCondition::evalCondition(uint16_t conditionHash16,
	const Variable& var1, const Variable& var2)
{
	switch (conditionHash16)
	{
	default:
	case str2int16("=="):
		return var1 == var2;
	case str2int16("!="):
		return var1 != var2;
	case str2int16(">"):
		return var1 > var2;
	case str2int16(">="):
		return var1 >= var2;
	case str2int16("<"):
		return var1 < var2;
	case str2int16("<="):
		return var1 <= var2;
	case str2int16("regex"):
	{
		bool regexMatch = false;
		if (std::holds_alternative<std::string>(var1) == true)
		{
			regexMatch = Regex::match(
				std::get<std::string>(var1).c_str(),
				VarUtils::toString(var2)
			);
		}
		return regexMatch;
	}
	}
}

void IfCondition::addCondition(ConditionOp op)
{
	elements.push_back(op);
}

void IfCondition::addCondition(uint16_t conditionHash16,
	const VarOrPredicate& param1, const VarOrPredicate& param2)
{
	Condition c;
	c.conditionHash16 = conditionHash16;
	c.param1 = param1;
	c.param2 = param2;
	elements.push_back(c);
}

bool IfCondition::eval(size_t& idx, const Game& game) const
{
	bool val = false;
	bool firstIteration = (idx == 0);
	ConditionOp currOp = ConditionOp::Or;

	while (idx < elements.size())
	{
		const auto& elem = elements[idx];
		idx++;
		bool val2 = false;
		if (std::holds_alternative<Condition>(elem) == true)
		{
			const auto& c = std::get<Condition>(elem);
			auto param1 = getVariable(game, c.param1);
			auto param2 = getVariable(game, c.param2);
			val2 = evalCondition(c.conditionHash16, param1, param2);
		}
		else
		{
			auto currOp2 = std::get<ConditionOp>(elem);
			switch (currOp2)
			{
			case ConditionOp::And:
			case ConditionOp::Or:
				currOp = currOp2;
				continue;
			case ConditionOp::LeftBracket:
				val2 = eval(idx, game);
				break;
			case ConditionOp::RightBracket:
				return val;
			default:
				continue;
			}
		}
		if (currOp == ConditionOp::And)
		{
			val = val & val2;
			if (firstIteration == true && val == false)
			{
				return false;
			}
		}
		else if (currOp == ConditionOp::Or)
		{
			val = val | val2;
		}
	}
	return val;
}

bool IfCondition::eval(const Game& game) const
{
	size_t idx = 0;
	return eval(idx, game);
}
