#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"
#include "Image.h"
#include "Parser/ParseVariable.h"
#include "Utils.h"

using Utils::str2int;

static Variable getVariable(Game& game, const Variable& var)
{
	if (var.is<std::string>() == true)
	{
		Variable var2(var);
		game.getVarOrProp(var.get<std::string>(), var2);
		return var2;
	}
	return var;
}

class ActIfCondition : public Action
{
private:
	unsigned conditionHash;
	Variable param1;
	Variable param2;
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
	ActIfCondition(unsigned conditionHash_,
		const Variable& param1_,
		const Variable& param2_,
		const std::shared_ptr<Action>& then_,
		const std::shared_ptr<Action>& else_)
		: conditionHash(conditionHash_), param1(param1_),
		param2(param2_), condThen(then_), condElse(else_) {}

	virtual bool execute(Game& game)
	{
		auto var1 = getVariable(game, param1);
		auto var2 = getVariable(game, param2);

		switch (conditionHash)
		{
		default:
		case str2int("=="):
			return ifCondition(game, var1 == var2);
		case str2int("!="):
			return ifCondition(game, var1 != var2);
		case str2int(">"):
			return ifCondition(game, var1 > var2);
		case str2int(">="):
			return ifCondition(game, var1 >= var2);
		case str2int("<"):
			return ifCondition(game, var1 < var2);
		case str2int("<="):
			return ifCondition(game, var1 <= var2);
		case str2int("fileExists"):
			return ifCondition(game,
				var1.is<std::string>() ?
				FileUtils::exists(var1.get<std::string>().c_str()) :
				false);
		}
		return true;
	}
};

class ActInListCondition : public Action
{
private:
	Variable var;
	std::vector<Variable> list;
	std::shared_ptr<Action> condThen;
	std::shared_ptr<Action> condElse;

public:
	ActInListCondition(Variable var_, const std::vector<Variable>& list_,
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
	Variable var;
	std::vector<std::pair<Variable, std::shared_ptr<Action>>> conditions;
	std::shared_ptr<Action> defaultAction;

public:
	ActSwitchCondition(Variable var_,
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
