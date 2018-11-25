#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"
#include "IfCondition.h"
#include "Image.h"
#include "Parser/ParseVariable.h"
#include "Utils/Utils.h"

class ActIfCondition : public Action
{
private:
	uint16_t conditionHash16;
	VarOrPredicate param1;
	VarOrPredicate param2;
	std::shared_ptr<Action> condThen;
	std::shared_ptr<Action> condElse;

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
		auto var1 = IfCondition::getVariable(game, param1);
		auto var2 = IfCondition::getVariable(game, param2);

		if (IfCondition::evalCondition(conditionHash16, var1, var2) == true)
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
			auto var1 = IfCondition::getVariable(game, var);
			for (const auto& elem : list)
			{
				if (var1 == IfCondition::getVariable(game, elem))
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

class ActMultiIfCondition : public Action
{
private:
	IfCondition conditions;
	std::shared_ptr<Action> condThen;
	std::shared_ptr<Action> condElse;

public:
	ActMultiIfCondition(const std::shared_ptr<Action>& then_,
		const std::shared_ptr<Action>& else_) : condThen(then_), condElse(else_) {}

	IfCondition& Conditions() noexcept { return conditions; }

	virtual bool execute(Game& game)
	{
		if (conditions.eval(game) == true)
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
			auto var1 = IfCondition::getVariable(game, var);
			for (const auto& elem : conditions)
			{
				if (var1 == IfCondition::getVariable(game, elem.first))
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
