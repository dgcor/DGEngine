#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"
#include "Image.h"
#include "Parser/ParseVariable.h"
#include "Utils.h"
#include "VarOrPredicate.h"

static Variable getVariable(Game& game, const VarOrPredicate& varOrPred)
{
	if (std::holds_alternative<Variable>(varOrPred) == true)
	{
		return game.getVarOrProp(std::get<Variable>(varOrPred));
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

class ActIfCondition : public Action
{
private:
	uint16_t conditionHash16;
	VarOrPredicate param1;
	VarOrPredicate param2;
	std::shared_ptr<Action> condThen;
	std::shared_ptr<Action> condElse;

	bool ifCondition(Game& game, bool condResult)
	{
		if (condResult == true)
		{
			if (condThen != nullptr)
			{
				return condThen->execute(game);
			}
		}
		else
		{
			if (condElse != nullptr)
			{
				return condElse->execute(game);
			}
		}
		return true;
	}

public:
	ActIfCondition(uint16_t conditionHash16_,
		const VarOrPredicate& param1_,
		const VarOrPredicate& param2_,
		const std::shared_ptr<Action>& then_,
		const std::shared_ptr<Action>& else_)
		: conditionHash16(conditionHash16_), param1(param1_),
		param2(param2_), condThen(then_), condElse(else_) {}

	virtual bool execute(Game& game)
	{
		auto var1 = getVariable(game, param1);
		auto var2 = getVariable(game, param2);

		switch (conditionHash16)
		{
		default:
		case str2int16("=="):
			return ifCondition(game, var1 == var2);
		case str2int16("!="):
			return ifCondition(game, var1 != var2);
		case str2int16(">"):
			return ifCondition(game, var1 > var2);
		case str2int16(">="):
			return ifCondition(game, var1 >= var2);
		case str2int16("<"):
			return ifCondition(game, var1 < var2);
		case str2int16("<="):
			return ifCondition(game, var1 <= var2);
		}
		return true;
	}
};

class ActInListCondition : public Action
{
private:
	VarOrPredicate var;
	std::vector<Variable> list;
	std::shared_ptr<Action> condThen;
	std::shared_ptr<Action> condElse;

public:
	ActInListCondition(VarOrPredicate var_, const std::vector<Variable>& list_,
		const std::shared_ptr<Action>& then_, const std::shared_ptr<Action>& else_)
		: var(var_), list(list_), condThen(then_), condElse(else_) {}

	virtual bool execute(Game& game)
	{
		if (list.empty() == false)
		{
			auto var1 = getVariable(game, var);
			for (const auto& elem : list)
			{
				if (var1 == getVariable(game, elem))
				{
					if (condThen != nullptr)
					{
						return condThen->execute(game);
					}
					return true;
				}
			}
		}
		if (condElse != nullptr)
		{
			return condElse->execute(game);
		}
		return true;
	}
};

class ActSwitchCondition : public Action
{
private:
	VarOrPredicate var;
	std::vector<std::pair<Variable, std::shared_ptr<Action>>> conditions;
	std::shared_ptr<Action> defaultAction;

public:
	ActSwitchCondition(VarOrPredicate var_,
		const std::vector<std::pair<Variable, std::shared_ptr<Action>>>& conditions_,
		const std::shared_ptr<Action>& defaultAction_)
		: var(var_), conditions(conditions_), defaultAction(defaultAction_) {}

	virtual bool execute(Game& game)
	{
		if (conditions.empty() == false)
		{
			auto var1 = getVariable(game, var);
			for (const auto& elem : conditions)
			{
				if (var1 == getVariable(game, elem.first))
				{
					if (elem.second != nullptr)
					{
						return elem.second->execute(game);
					}
					return true;
				}
			}
		}
		if (defaultAction != nullptr)
		{
			return defaultAction->execute(game);
		}
		return true;
	}
};
