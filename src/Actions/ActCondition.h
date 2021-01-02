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

	bool execute(Game& game) override
	{
		auto var1 = IfCondition::getVariable(game, param1);
		auto var2 = IfCondition::getVariable(game, param2);

		if (IfCondition::evalCondition(conditionHash16, var1, var2) == true)
		{
			if (condThen != nullptr)
			{
				condThen->execute(game);
			}
			return true;
		}
		else
		{
			if (condElse != nullptr)
			{
				condElse->execute(game);
			}
			return false;
		}
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
	ActInListCondition(const VarOrPredicate& var_, std::vector<Variable>&& list_,
		const std::shared_ptr<Action>& then_, const std::shared_ptr<Action>& else_)
		: var(var_), list(std::move(list_)), condThen(then_), condElse(else_) {}

	bool execute(Game& game) override
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
						condThen->execute(game);
					}
					return true;
				}
			}
		}
		if (condElse != nullptr)
		{
			condElse->execute(game);
		}
		return false;
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

	bool execute(Game& game) override
	{
		if (conditions.eval(game) == true)
		{
			if (condThen != nullptr)
			{
				condThen->execute(game);
			}
			return true;
		}
		else
		{
			if (condElse != nullptr)
			{
				condElse->execute(game);
			}
			return false;
		}
	}
};

class ActSwitchCondition : public Action
{
private:
	VarOrPredicate var;
	std::vector<std::pair<Variable, std::shared_ptr<Action>>> conditions;
	std::shared_ptr<Action> defaultAction;

public:
	ActSwitchCondition(const VarOrPredicate& var_,
		std::vector<std::pair<Variable, std::shared_ptr<Action>>>&& conditions_,
		const std::shared_ptr<Action>& defaultAction_)
		: var(var_), conditions(std::move(conditions_)), defaultAction(defaultAction_) {}

	bool execute(Game& game) override
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
						elem.second->execute(game);
					}
					return true;
				}
			}
		}
		if (defaultAction != nullptr)
		{
			defaultAction->execute(game);
		}
		return false;
	}
};
