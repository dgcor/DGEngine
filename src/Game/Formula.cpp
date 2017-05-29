#include "Formula.h"
#include "Utils.h"

Formula::Formula(const std::string& formula)
{
	// tokenize formula
	const std::string delimiters{ "+-*/()" };
	std::string buffer;
	std::vector<std::string> tokens;
	for (auto c : formula)
	{
		if (c == ' ')
		{
			continue;
		}
		if (delimiters.find(c) != std::string::npos)
		{
			if (buffer.size() > 0)
			{
				tokens.push_back(buffer);
			}
			tokens.push_back(std::string(1, c));
			buffer.clear();
		}
		else
		{
			buffer += c;
		}
	}
	if (buffer.size() > 0 && buffer != ")")
	{
		tokens.push_back(buffer);
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
			try
			{
				elements.push_back({ std::stod(elem) });
			}
			catch (std::out_of_range e)
			{
				elements.push_back({ 0.0 });
			}
			catch (std::invalid_argument e)
			{
				elements.push_back({ elem });
			}
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
		if (elem.is<double>() == true)
		{
			val2 = elem.get<double>();
		}
		else if (elem.is<std::string>() == true)
		{
			const auto& prop = elem.get<std::string>();
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
			switch (elem.get<FormulaOp>())
			{
			case FormulaOp::LeftBracket:
				val2 = eval(idx, queryA, queryB);
				break;
			case FormulaOp::RightBracket:
				return val;
			default:
				currFormula = elem.get<FormulaOp>();
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
		if (elem.is<double>() == true)
		{
			str += Utils::toString(elem.get<double>()) + ' ';
			continue;
		}
		else if (elem.is<std::string>() == true)
		{
			str += elem.get<std::string>() + ' ';
			continue;
		}
		switch (elem.get<FormulaOp>())
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
