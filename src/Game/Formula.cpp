#include "Formula.h"
#if (_MSC_VER >= 1914)
#include <charconv>
#else
#include <cstdlib>
#endif
#include "Utils/Utils.h"

Formula::Formula(const std::string_view formula)
{
	// tokenize formula
	const std::string_view delimiters{ "+-*/()" };
	size_t bufferStart = 0;
	size_t bufferSize = 0;
	std::vector<std::string_view> tokens;
	for (size_t i = 0; i < formula.size(); i++)
	{
		auto c = formula[i];
		if (c == ' ')
		{
			if (bufferSize == 0)
			{
				bufferStart++;
			}
			continue;
		}
		if (delimiters.find(c) != std::string::npos)
		{
			if (bufferSize > 0)
			{
				tokens.push_back(std::string_view(formula.data() + bufferStart, bufferSize));
			}
			tokens.push_back(std::string_view(&formula[i], 1));
			bufferStart = i + 1;
			bufferSize = 0;
		}
		else
		{
			bufferSize++;
		}
	}
	if (bufferSize > 0)
	{
		auto token = std::string_view(formula.data() + bufferStart, bufferSize);
		if (token != ")")
		{
			tokens.push_back(token);
		}
	}

	// create internal formula
	int brackets = 0;
	for (const auto& elem : tokens)
	{
		if (elem.empty() == true)
		{
			continue;
		}
		if (elem == "+")
		{
			elements.push_back({ FormulaOp::Add });
		}
		else if (elem == "-")
		{
			elements.push_back({ FormulaOp::Subtract });
		}
		else if (elem == "*")
		{
			elements.push_back({ FormulaOp::Multiply });
		}
		else if (elem == "/")
		{
			elements.push_back({ FormulaOp::Divide });
		}
		else if (elem == "(")
		{
			elements.push_back({ FormulaOp::LeftBracket });
			brackets++;
		}
		else if (elem == ")")
		{
			if (brackets > 0)
			{
				elements.push_back({ FormulaOp::RightBracket });
				brackets--;
			}
		}
		else
		{
#if (_MSC_VER >= 1914)
			double val = 0.0;
			auto err = std::from_chars(elem.data(), elem.data() + elem.size(), val);
			if (err.ec == std::errc() ||
				err.ec == std::errc::result_out_of_range)
			{
				elements.push_back({ val });
			}
			else
			{
				elements.push_back(std::string(elem));
			}
#else
			int& errno_ref = errno;
			const char* sPtr = elem.data();
			char* ePtr;
			errno_ref = 0;
			double val = std::strtod(sPtr, &ePtr);

			if (errno_ref == ERANGE) // out of range
			{
				elements.push_back({ 0.0 });
			}
			else if (sPtr == ePtr) // invalid argument
			{
				elements.push_back(std::string(elem));
			}
			else
			{
				elements.push_back({ val });
			}
#endif
		}
	}
	// remove trailing closing brackets (faster eval, same result)
	while (elements.empty() == false &&
		elements.back() == FormulaElement(FormulaOp::RightBracket))
	{
		elements.pop_back();
	}
}

double Formula::eval(size_t& idx, const LevelObject& queryA, const LevelObject& queryB) const
{
	double val = 0.0;
	FormulaOp currFormula = FormulaOp::Add;

	while (idx < elements.size())
	{
		const auto& elem = elements[idx];
		idx++;
		double val2 = 0.0;
		if (std::holds_alternative<double>(elem) == true)
		{
			val2 = std::get<double>(elem);
		}
		else if (std::holds_alternative<std::string>(elem) == true)
		{
			const auto& prop = std::get<std::string>(elem);
			if (prop.empty() == false)
			{
				Number32 queryVal;
				if (prop[0] != '$')
				{
					if (queryA.getNumberProp(prop.c_str(), queryVal) == true)
					{
						val2 = queryVal.getDouble();
					}
				}
				else
				{
					if (queryB.getNumberProp(prop.c_str() + 1, queryVal) == true)
					{
						val2 = queryVal.getDouble();
					}
				}
			}
		}
		else
		{
			switch (std::get<FormulaOp>(elem))
			{
			case FormulaOp::LeftBracket:
				val2 = eval(idx, queryA, queryB);
				break;
			case FormulaOp::RightBracket:
				return val;
			default:
				currFormula = std::get<FormulaOp>(elem);
				continue;
			}
		}
		switch (currFormula)
		{
		default:
		case FormulaOp::Add:
			val = val + val2;
			break;
		case FormulaOp::Subtract:
			val = val - val2;
			break;
		case FormulaOp::Multiply:
			val = val * val2;
			break;
		case FormulaOp::Divide:
			val = val / (val2 != 0.0 ? val2 : 1.0);
			break;
		}
	}
	return val;
}

double Formula::eval(const LevelObject& queryA, const LevelObject& queryB) const
{
	size_t idx = 0;
	return eval(idx, queryA, queryB);
}

double Formula::eval(const LevelObject& query) const
{
	return eval(query, query);
}

std::string Formula::toString() const
{
	std::string str;
	int brackets = 0;

	for (const auto elem : elements)
	{
		if (std::holds_alternative<double>(elem) == true)
		{
			str += Utils::toString(std::get<double>(elem)) + ' ';
			continue;
		}
		else if (std::holds_alternative<std::string>(elem) == true)
		{
			str += std::get<std::string>(elem) + ' ';
			continue;
		}
		switch (std::get<FormulaOp>(elem))
		{
		case FormulaOp::Add:
		{
			str += "+ ";
			break;
		}
		case FormulaOp::Subtract:
		{
			str += "- ";
			break;
		}
		case FormulaOp::Multiply:
		{
			str += "* ";
			break;
		}
		case FormulaOp::Divide:
		{
			str += "/ ";
			break;
		}
		case FormulaOp::LeftBracket:
		{
			str += '(';
			brackets++;
			break;
		}
		case FormulaOp::RightBracket:
		{
			if (brackets > 0)
			{
				str = Utils::trimEnd(str);
				str += ") ";
				brackets--;
			}
			break;
		}
		}
	}
	str = Utils::trimEnd(str);
	while (brackets > 0)
	{
		str += ')';
		brackets--;
	}
	return str;
}
