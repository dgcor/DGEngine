#pragma once

#include "LevelObject.h"
#include <string>
#include <string_view>
#include <variant>
#include <vector>

// use brackets to force order
// ex: 2 + 2 * 4 = 16
// ex: 2 + (2 * 4) = 10
class Formula
{
private:
	enum class FormulaOp
	{
		Add,
		Subtract,
		Multiply,
		Divide,
		LeftBracket,
		RightBracket,
	};
	typedef std::variant<FormulaOp, double, std::string> FormulaElement;

	std::vector<FormulaElement> elements;

	double eval(size_t& idx, const LevelObject& queryA, const LevelObject& queryB) const;

public:
	Formula() noexcept {}
	Formula(const std::string_view formula);

	bool empty() const  noexcept { return elements.empty(); }

	double eval(const LevelObject& query) const;
	double eval(const LevelObject& queryA, const LevelObject& queryB) const;

	std::string toString() const;
};
