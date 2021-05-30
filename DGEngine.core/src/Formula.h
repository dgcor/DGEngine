#pragma once

#include "Queryable.h"
#include <string>
#include <string_view>
#include <variant>
#include <vector>

// use brackets to force order
// ex: 2 + 2 * 4 = 16
// ex: 2 + (2 * 4) = 10
// ex: 2 - :abs(-10) = -8
// ops:
// +        add
// -        Subtract
// *        Multiply
// /        Divide
// %        Modulo
// :mod     Modulo
// ^        Power
// :pow     Power
// :min     Min (ex: (2 + 2) :min 10 = 4)
// :max     Max (ex: (2 + 2) :max 10 = 10)
// :zero    null value
// :nvl     null value (ex: (2 - 2) :nvl 10 = 10 | (2 - 1) :nvl 10 = 1)
// :neg     negative value (ex: (2 - 3) :neg 10 = 10 | (2 - 1) :neg 10 = 1)
// :negz    negative or zero value (ex: (2 - 2) :negz 10 = 10 | (2 - 1) :negz 10 = 1)
// :pos     positive value (ex: (2 - 1) :pos 10 = 10 | (2 - 3) :pos 10 = -1)
// :posz    positive or zero value (ex: (2 - 2) :posz 10 = 10 | (2 - 3) :posz 10 = -1)
// :rnd     random number (ex: :rnd(10) = 0 to 10)
// :rndf    random number (ex: :rnd(10) = 0.0 to 9.9)
// :rndn    normal distribution random number (ex: :rnd(10) = 0 to 10)
// :abs     absolute value
// :ceil    round value up
// :floor   round value down
// :trunc   round value towards 0
// :round   round value
// :log     common logarithm
// :ln      natural logarithm
// :sqrt    square root
// :cos     cosine
// :sin     sine
// :tan     tangent
class Formula
{
private:
	enum class FormulaOp
	{
		None,
		Add,
		Subtract,
		Multiply,
		Divide,
		Modulo,
		Power,
		Min,
		Max,
		Nvl,
		Negative,
		NegativeOr0,
		Positive,
		PositiveOr0,
		Rand,
		RandFloat,
		RandNormalDist,
		Abs,
		Ceil,
		Floor,
		Trunc,
		Round,
		Log,
		Ln,
		Sqrt,
		Cos,
		Sin,
		Tan,
		LeftBracket,
		RightBracket,
	};

	typedef std::variant<FormulaOp, double, std::string, std::string_view> FormulaElement;

	std::vector<FormulaElement> elements;

	struct VectorIterator
	{
		std::vector<FormulaElement>::const_iterator it;
		std::vector<FormulaElement>::const_iterator end;

		VectorIterator(const std::vector<FormulaElement>& elements_) :
			it(elements_.cbegin()), end(elements_.cend()) {}
	};
	struct FormulaIterator
	{
		const std::string_view formula;
		uint16_t bufferStart{ 0 };
		uint16_t bufferSize{ 0 };
		int16_t i{ 0 };
		FormulaElement element{ FormulaOp::None };

		FormulaIterator(const std::string_view formula_, bool getStringRefs = true);

		const FormulaElement& get() const { return element; }
		bool isValid() const { return i >= 0; }
		void next(bool getStringRefs = true);
	};

	class FormulaElementIterator
	{
	private:
		std::variant<VectorIterator, FormulaIterator> it;

	public:
		FormulaElementIterator(const std::vector<FormulaElement>& elements_)
			: it(elements_) {}

		FormulaElementIterator(const std::string_view formula_)
			: it(formula_) {}

		const FormulaElement& get() const;
		bool isValid() const;
		void next();
	};

	static FormulaElement parseToken(const std::string_view token, bool getStringRefs);

	// skip tokens to the right of a binary op if the left part won't be affected by the right.
	// returns the index of the next token to process or the size of the formula if at the end.
	static void skipTokens(FormulaElementIterator& it);

	static double eval(FormulaElementIterator& it, const Queryable* queryA,
		const Queryable* queryB, int32_t randomNum);

	double evalMinMax(const Queryable* queryA,
		const Queryable* queryB, const std::string_view minMaxNum) const;

public:
	Formula() noexcept {}
	Formula(const std::string_view formula);

	bool empty() const  noexcept { return elements.empty(); }

	// randomNum - random number to use
	// randomNum > 0 -> use given number (ex: :rnd(10) = randomNum)
	// randomNum = 0 -> disabled (ex: :rnd(10) = 0-9)
	// randomNum = -1 -> max - 1  (ex: :rnd(10) = 9)
	// randomNum <= -2 -> 0        (ex: :rnd(10) = 0)
	double eval(int32_t randomNum = 0) const;
	double eval(const Queryable& query, int32_t randomNum = 0) const;
	double eval(const Queryable& queryA, const Queryable& queryB,
		int32_t randomNum = 0) const;

	// minMaxNum - string_view with random number to use
	// minMaxNum > 0 -> use given number (ex: :rnd(10) = randomNum)
	// minMaxNum = 0 -> disabled (ex: :rnd(10) = 0-9)
	// minMaxNum = max -> max - 1  (ex: :rnd(10) = 9)
	// minMaxNum = min -> 0        (ex: :rnd(10) = 0)
	double eval(const std::string_view minMaxNum) const;
	double eval(const Queryable& query, const std::string_view minMaxNum) const;
	double eval(const Queryable& queryA, const Queryable& queryB,
		const std::string_view minMaxNum) const;

	static double evalString(const std::string_view formula, int32_t randomNum = 0);
	static double evalString(const std::string_view formula,
		const Queryable& query, int32_t randomNum = 0);
	static double evalString(const std::string_view formula,
		const Queryable* query, int32_t randomNum = 0);
	static double evalString(const std::string_view formula,
		const Queryable& queryA, const Queryable& queryB, int32_t randomNum = 0);
	static double evalString(const std::string_view formula,
		const Queryable* queryA, const Queryable* queryB, int32_t randomNum = 0);

	static double evalMinString(const std::string_view formula)
	{
		return evalString(formula, -2);
	}
	static double evalMinString(const std::string_view formula, const Queryable& query)
	{
		return evalString(formula, query, -2);
	}
	static double evalMinString(const std::string_view formula, const Queryable* query)
	{
		return evalString(formula, query, -2);
	}
	static double evalMinString(const std::string_view formula,
		const Queryable& queryA, const Queryable& queryB)
	{
		return evalString(formula, queryA, queryB, -2);
	}
	static double evalMinString(const std::string_view formula,
		const Queryable* queryA, const Queryable* queryB)
	{
		return evalString(formula, queryA, queryB, -2);
	}

	static double evalMaxString(const std::string_view formula)
	{
		return evalString(formula, -1);
	}
	static double evalMaxString(const std::string_view formula, const Queryable& query)
	{
		return evalString(formula, query, -1);
	}
	static double evalMaxString(const std::string_view formula, const Queryable* query)
	{
		return evalString(formula, query, -1);
	}
	static double evalMaxString(const std::string_view formula,
		const Queryable& queryA, const Queryable& queryB)
	{
		return evalString(formula, queryA, queryB, -1);
	}
	static double evalMaxString(const std::string_view formula,
		const Queryable* queryA, const Queryable* queryB)
	{
		return evalString(formula, queryA, queryB, -1);
	}

	std::string toString() const;
};
