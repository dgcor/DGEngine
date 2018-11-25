#pragma once

#include "VarOrPredicate.h"
#include <vector>

class Game;

// use brackets to force order
// ex: var1 == 2 and (var2 == 3 or var2 = 4)
class IfCondition
{
public:
	enum class ConditionOp
	{
		And,
		Or,
		LeftBracket,
		RightBracket,
	};

private:
	struct Condition
	{
		uint16_t conditionHash16{ 0 };
		VarOrPredicate param1;
		VarOrPredicate param2;
	};

	typedef std::variant<ConditionOp, Condition> ConditionElement;

	std::vector<ConditionElement> elements;

	bool eval(size_t& idx, const Game& game) const;

public:
	static Variable getVariable(const Game& game, const VarOrPredicate& varOrPred);

	static bool evalCondition(uint16_t conditionHash16,
		const Variable& var1, const Variable& var2);

	void addCondition(ConditionOp op);
	void addCondition(uint16_t conditionHash16,
		const VarOrPredicate& param1, const VarOrPredicate& param2);

	bool empty() const  noexcept { return elements.empty(); }

	bool eval(const Game& game) const;
};
