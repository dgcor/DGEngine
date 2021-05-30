#include "Formula.h"
#include <charconv>
#include <cmath>
#include "Utils/Utils.h"

Formula::FormulaIterator::FormulaIterator(const std::string_view formula_,
	bool getStringRefs) : formula(formula_)
{
	if (formula.size() >= (size_t)std::numeric_limits<int16_t>::max())
	{
		i = -1;
	}
	else
	{
		next(getStringRefs);
	}
}

void Formula::FormulaIterator::next(bool getStringRefs)
{
	if (i < 0)
	{
		return;
	}
	// tokenize formula
	static const std::string_view delimiters{ " +-*/^:()" };
	for (; (size_t)i < formula.size(); i++)
	{
		auto c = formula[i];
		if (delimiters.find(c) != std::string::npos)
		{
			if (bufferSize > 0)
			{
				element = parseToken(
					std::string_view(formula.data() + bufferStart, bufferSize),
					getStringRefs
				);
				bufferSize = 0;
				return;
			}
			if (c == ':')
			{
				bufferStart = i;
				bufferSize = 1;
			}
			else
			{
				bufferStart = i + 1;
				bufferSize = 0;
				if (c != ' ')
				{
					element = parseToken(
						std::string_view(&formula[i], 1),
						getStringRefs
					);
					i++;
					return;
				}
			}
		}
		else
		{
			bufferSize++;
		}
	}
	if (bufferSize > 0)
	{
		element = parseToken(
			std::string_view(formula.data() + bufferStart, bufferSize),
			getStringRefs
		);
		bufferSize = 0;
		return;
	}
	i = -1;
}

const Formula::FormulaElement& Formula::FormulaElementIterator::get() const
{
	if (std::holds_alternative<VectorIterator>(it) == true)
	{
		return *std::get<VectorIterator>(it).it;
	}
	return std::get<FormulaIterator>(it).get();
}

bool Formula::FormulaElementIterator::isValid() const
{
	if (std::holds_alternative<VectorIterator>(it) == true)
	{
		return std::get<VectorIterator>(it).it < std::get<VectorIterator>(it).end;
	}
	return std::get<FormulaIterator>(it).isValid();
}

void Formula::FormulaElementIterator::next()
{
	if (std::holds_alternative<VectorIterator>(it) == true)
	{
		if (std::get<VectorIterator>(it).it < std::get<VectorIterator>(it).end)
		{
			++std::get<VectorIterator>(it).it;
		}
		return;
	}
	std::get<FormulaIterator>(it).next();
}

Formula::Formula(const std::string_view formula)
{
	// create internal formula
	FormulaIterator it(formula, false);
	int brackets = 0;
	for (; it.isValid() == true; it.next(false))
	{
		const auto& elem = it.get();
		if (std::holds_alternative<FormulaOp>(elem) == true)
		{
			auto formulaOp = std::get<FormulaOp>(elem);
			if (formulaOp == FormulaOp::None)
			{
				continue;
			}
			else if (formulaOp == FormulaOp::LeftBracket)
			{
				brackets++;
			}
			else if (formulaOp == FormulaOp::RightBracket)
			{
				if (brackets <= 0)
				{
					continue;
				}
				brackets--;
			}
		}
		elements.push_back(elem);
	}
	// remove trailing closing brackets (faster eval, same result)
	while (elements.empty() == false &&
		elements.back() == FormulaElement(FormulaOp::RightBracket))
	{
		elements.pop_back();
	}
}

Formula::FormulaElement Formula::parseToken(const std::string_view token, bool getStringRefs)
{
	if (token.empty() == true)
	{
		return FormulaOp::None;
	}
	if (token.size() == 1 || token[0] == ':')
	{
		switch (str2int16(token))
		{
		case str2int16("+"):
			return FormulaOp::Add;
		case str2int16("-"):
			return FormulaOp::Subtract;
		case str2int16("*"):
			return FormulaOp::Multiply;
		case str2int16("/"):
			return FormulaOp::Divide;
		case str2int16("%"):
		case str2int16(":mod"):
			return FormulaOp::Modulo;
		case str2int16("^"):
		case str2int16(":pow"):
			return FormulaOp::Power;
		case str2int16(":min"):
			return FormulaOp::Min;
		case str2int16(":max"):
			return FormulaOp::Max;
		case str2int16(":nvl"):
		case str2int16(":zero"):
			return FormulaOp::Nvl;
		case str2int16(":neg"):
			return FormulaOp::Negative;
		case str2int16(":negz"):
			return FormulaOp::NegativeOr0;
		case str2int16(":pos"):
			return FormulaOp::Positive;
		case str2int16(":posz"):
			return FormulaOp::PositiveOr0;
		case str2int16(":rnd"):
			return FormulaOp::Rand;
		case str2int16(":rndf"):
			return FormulaOp::RandFloat;
		case str2int16(":rndn"):
			return FormulaOp::RandNormalDist;
		case str2int16(":abs"):
			return FormulaOp::Abs;
		case str2int16(":ceil"):
			return FormulaOp::Ceil;
		case str2int16(":floor"):
			return FormulaOp::Floor;
		case str2int16(":trunc"):
			return FormulaOp::Trunc;
		case str2int16(":round"):
			return FormulaOp::Round;
		case str2int16(":log"):
			return FormulaOp::Log;
		case str2int16(":ln"):
			return FormulaOp::Ln;
		case str2int16(":sqrt"):
			return FormulaOp::Sqrt;
		case str2int16(":cos"):
			return FormulaOp::Cos;
		case str2int16(":sin"):
			return FormulaOp::Sin;
		case str2int16(":tan"):
			return FormulaOp::Tan;
		case str2int16("("):
			return FormulaOp::LeftBracket;
		case str2int16(")"):
			return FormulaOp::RightBracket;
		default:
			break;
		}
	}

	double val = 0.0;
	auto err = std::from_chars(token.data(), token.data() + token.size(), val);
	if (err.ec == std::errc() ||
		err.ec == std::errc::result_out_of_range)
	{
		return val;
	}
	else
	{
		return getStringRefs ? FormulaElement(token) : FormulaElement(std::string(token));
	}
}

void Formula::skipTokens(FormulaElementIterator& it)
{
	// this functions skips:
	// left bracket (if first element) until the final right bracket is found
	// unary ops (if first element is a binary op) and whatever comes after (value or left bracket)
	// string (query) or value (double)

	// examples of functions that will skip:
	// 0 * (2 + 1 * 3)
	// 0 / mana
	// 0 / :log 10
	// 0 * (-20) ---> this is equivalent to ---> 0 * (0 - 20)

	// examples of functions that won't skip:
	// 0 * -20 ---> this is equivalent to ---> 0 - 20
	// 0 :nvl -20 ---> this is equivalent to ---> 0 - 20

	it.next();
	while (it.isValid() == true)
	{
		const auto& elem = it.get();
		if (std::holds_alternative<double>(elem) == true ||
			std::holds_alternative<std::string>(elem) == true ||
			std::holds_alternative<std::string_view>(elem) == true)
		{
			break;
		}
		else
		{
			switch (std::get<FormulaOp>(elem))
			{
			case FormulaOp::Rand:
			case FormulaOp::RandFloat:
			case FormulaOp::RandNormalDist:
			case FormulaOp::Abs:
			case FormulaOp::Ceil:
			case FormulaOp::Floor:
			case FormulaOp::Trunc:
			case FormulaOp::Round:
			case FormulaOp::Log:
			case FormulaOp::Ln:
			case FormulaOp::Sqrt:
			case FormulaOp::Cos:
			case FormulaOp::Sin:
			case FormulaOp::Tan:
			{
				skipTokens(it);
				break;
			}
			case FormulaOp::LeftBracket:
			{
				int nestedBrackets = 1;
				it.next();
				while (it.isValid() == true)
				{
					const auto& elem2 = it.get();
					if (std::holds_alternative<FormulaOp>(elem2) == true)
					{
						if (std::get<FormulaOp>(elem2) == FormulaOp::RightBracket)
						{
							nestedBrackets--;
							if (nestedBrackets == 0)
							{
								break;
							}
						}
						else if (std::get<FormulaOp>(elem2) == FormulaOp::LeftBracket)
						{
							nestedBrackets++;
						}
					}
					it.next();
				}
				break;
			}
			default:
				break;
			}
		}
		break;
	}
}

double Formula::eval(FormulaElementIterator& it, const Queryable* queryA,
	const Queryable* queryB, int32_t randomNum)
{
	double val = 0.0;
	FormulaOp currUnaryOp = FormulaOp::None;
	FormulaOp currBinaryOp = FormulaOp::Add;

	for (; it.isValid() == true; it.next())
	{
		const auto& elem = it.get();
		double val2 = 0.0;
		if (std::holds_alternative<double>(elem) == true)
		{
			val2 = std::get<double>(elem);
		}
		else if (std::holds_alternative<std::string>(elem) == true ||
			std::holds_alternative<std::string_view>(elem) == true)
		{
			std::string_view prop;
			if (std::holds_alternative<std::string>(elem) == true)
			{
				prop = std::get<std::string>(elem);
			}
			else
			{
				prop = std::get<std::string_view>(elem);
			}
			if (prop.empty() == false)
			{
				Number32 queryVal;
				if (prop[0] != '$')
				{
					if (queryA != nullptr &&
						queryA->getNumberProp(prop, queryVal) == true)
					{
						val2 = queryVal.getDouble();
					}
				}
				else
				{
					if (queryB != nullptr &&
						queryB->getNumberProp({ prop.data() + 1 , prop.size() - 1 }, queryVal) == true)
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
			{
				it.next();
				val2 = eval(it, queryA, queryB, randomNum);
				break;
			}
			case FormulaOp::RightBracket:
				return val;
			default:
			{
				auto currOp = std::get<FormulaOp>(elem);
				switch (currOp)
				{
				case FormulaOp::Add:
				case FormulaOp::Subtract:
				case FormulaOp::Multiply:
				case FormulaOp::Divide:
				case FormulaOp::Modulo:
				case FormulaOp::Power:
				case FormulaOp::Min:
				case FormulaOp::Max:
				case FormulaOp::Nvl:
				case FormulaOp::Negative:
				case FormulaOp::NegativeOr0:
				case FormulaOp::Positive:
				case FormulaOp::PositiveOr0:
				{
					// optimization - skip tokens for these ops
					if ((currOp == FormulaOp::Multiply ||
						currOp == FormulaOp::Divide ||
						currOp == FormulaOp::Power) &&
						val == 0.0)
					{
						skipTokens(it);
					}
					else if (val != 0.0 && currOp == FormulaOp::Nvl)
					{
						skipTokens(it);
					}
					else if (val >= 0.0 && currOp == FormulaOp::Negative)
					{
						skipTokens(it);
					}
					else if (val > 0.0 && currOp == FormulaOp::NegativeOr0)
					{
						skipTokens(it);
					}
					else if (val <= 0.0 && currOp == FormulaOp::Positive)
					{
						skipTokens(it);
					}
					else if (val < 0.0 && currOp == FormulaOp::PositiveOr0)
					{
						skipTokens(it);
					}

					currBinaryOp = currOp;
					currUnaryOp = FormulaOp::None;
					break;
				}
				default:
				{
					currUnaryOp = currOp;
					break;
				}
				}
				continue;
			}
			}
		}
		switch (currUnaryOp)
		{
		case FormulaOp::Rand:
		case FormulaOp::RandFloat:
		{
			if (randomNum == 0)
			{
				if (val2 > 0.0)
				{
					if (currUnaryOp == FormulaOp::Rand)
					{
						val2 = (double)Utils::Random::get((uint32_t)val2);
					}
					else
					{
						val2 = Utils::Random::getf(val2);
					}
				}
				else
				{
					val2 = 0.0;
				}
			}
			else if (randomNum == -1)
			{
				val2 = std::max(0.0, val2 - 1.0);
			}
			else if (randomNum < -1)
			{
				val2 = 0.0;
			}
			else
			{
				val2 = (double)randomNum;
			}
			break;
		}
		case FormulaOp::RandNormalDist:
		{
			if (randomNum == 0)
			{
				if (val2 > 0.0)
				{
					val2 = std::round(Utils::RandomNormal::getRange(val2));
				}
				else
				{
					val2 = 0.0;
				}
			}
			else if (randomNum == -1)
			{
				val2 = std::max(0.0, val2);
			}
			else if (randomNum < -1)
			{
				val2 = 0.0;
			}
			else
			{
				val2 = (double)randomNum;
			}
			break;
		}
		case FormulaOp::Abs:
			val2 = std::abs(val2);
			break;
		case FormulaOp::Ceil:
			val2 = std::ceil(val2);
			break;
		case FormulaOp::Floor:
			val2 = std::floor(val2);
			break;
		case FormulaOp::Trunc:
			val2 = std::trunc(val2);
			break;
		case FormulaOp::Round:
			val2 = std::round(val2);
			break;
		case FormulaOp::Log:
			val2 = std::log10(val2);
			break;
		case FormulaOp::Ln:
			val2 = std::log(val2);
			break;
		case FormulaOp::Sqrt:
			val2 = std::sqrt(val2);
			break;
		case FormulaOp::Cos:
			val2 = std::cos(val2);
			break;
		case FormulaOp::Sin:
			val2 = std::sin(val2);
			break;
		case FormulaOp::Tan:
			val2 = std::tan(val2);
			break;
		default:
			break;
		}
		switch (currBinaryOp)
		{
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
		case FormulaOp::Modulo:
			val = std::fmod(val, val2);
			break;
		case FormulaOp::Power:
			val = std::pow(val, val2);
			break;
		case FormulaOp::Min:
			val = std::min(val, val2);
			break;
		case FormulaOp::Max:
			val = std::max(val, val2);
			break;
		case FormulaOp::Nvl:
			val = (val == 0.0 ? val2 : val);
			break;
		case FormulaOp::Negative:
			val = (val < 0.0 ? val2 : val);
			break;
		case FormulaOp::NegativeOr0:
			val = (val <= 0.0 ? val2 : val);
			break;
		case FormulaOp::Positive:
			val = (val > 0.0 ? val2 : val);
			break;
		case FormulaOp::PositiveOr0:
			val = (val >= 0.0 ? val2 : val);
			break;
		default:
			break;
		}
	}
	return val;
}

double Formula::evalMinMax(const Queryable* queryA,
	const Queryable* queryB, const std::string_view minMaxNum) const
{
	FormulaElementIterator it(elements);
	int32_t randomNum;
	if (minMaxNum.empty() == true || minMaxNum == "0")
	{
		randomNum = 0;
	}
	else if (minMaxNum == "max")
	{
		randomNum = -1;
	}
	else if (minMaxNum == "min")
	{
		randomNum = -2;
	}
	else
	{
		randomNum = Utils::strtonumber<int32_t>(minMaxNum);
	}
	return eval(it, queryA, queryB, randomNum);
}

double Formula::eval(const Queryable& queryA, const Queryable& queryB, int32_t randomNum) const
{
	FormulaElementIterator it(elements);
	return eval(it, &queryA, &queryB, randomNum);
}

double Formula::eval(const Queryable& query, int32_t randomNum) const
{
	FormulaElementIterator it(elements);
	return eval(it, &query, &query, randomNum);
}

double Formula::eval(int32_t randomNum) const
{
	FormulaElementIterator it(elements);
	return eval(it, nullptr, nullptr, randomNum);
}

double Formula::eval(const Queryable& queryA, const Queryable& queryB,
	const std::string_view minMaxNum) const
{
	return evalMinMax(&queryA, &queryB, minMaxNum);
}

double Formula::eval(const Queryable& query, const std::string_view minMaxNum) const
{
	return evalMinMax(&query, &query, minMaxNum);
}

double Formula::eval(const std::string_view minMaxNum) const
{
	return evalMinMax(nullptr, nullptr, minMaxNum);
}

double Formula::evalString(const std::string_view formula, int32_t randomNum)
{
	FormulaElementIterator it(formula);
	return eval(it, nullptr, nullptr, randomNum);
}

double Formula::evalString(const std::string_view formula,
	const Queryable& query, int32_t randomNum)
{
	FormulaElementIterator it(formula);
	return eval(it, &query, &query, randomNum);
}

double Formula::evalString(const std::string_view formula,
	const Queryable* query, int32_t randomNum)
{
	FormulaElementIterator it(formula);
	return eval(it, query, query, randomNum);
}

double Formula::evalString(const std::string_view formula,
	const Queryable& queryA, const Queryable& queryB, int32_t randomNum)
{
	FormulaElementIterator it(formula);
	return eval(it, &queryA, &queryB, randomNum);
}

double Formula::evalString(const std::string_view formula,
	const Queryable* queryA, const Queryable* queryB, int32_t randomNum)
{
	FormulaElementIterator it(formula);
	return eval(it, queryA, queryB, randomNum);
}

std::string Formula::toString() const
{
	std::string str;
	int brackets = 0;

	for (const auto& elem : elements)
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
			str += "+ ";
			break;
		case FormulaOp::Subtract:
			str += "- ";
			break;
		case FormulaOp::Multiply:
			str += "* ";
			break;
		case FormulaOp::Divide:
			str += "/ ";
			break;
		case FormulaOp::Modulo:
			str += "% ";
			break;
		case FormulaOp::Power:
			str += "^ ";
			break;
		case FormulaOp::Min:
			str += ":min ";
			break;
		case FormulaOp::Max:
			str += ":max ";
			break;
		case FormulaOp::Nvl:
			str += ":nvl ";
			break;
		case FormulaOp::Negative:
			str += ":neg ";
			break;
		case FormulaOp::NegativeOr0:
			str += ":negz ";
			break;
		case FormulaOp::Positive:
			str += ":pos ";
			break;
		case FormulaOp::PositiveOr0:
			str += ":posz ";
			break;
		case FormulaOp::Rand:
			str += ":rnd ";
			break;
		case FormulaOp::RandFloat:
			str += ":rndf ";
			break;
		case FormulaOp::RandNormalDist:
			str += ":rndn ";
			break;
		case FormulaOp::Abs:
			str += ":abs ";
			break;
		case FormulaOp::Ceil:
			str += ":ceil ";
			break;
		case FormulaOp::Floor:
			str += ":floor ";
			break;
		case FormulaOp::Trunc:
			str += ":trunc ";
			break;
		case FormulaOp::Round:
			str += ":round ";
			break;
		case FormulaOp::Log:
			str += ":log ";
			break;
		case FormulaOp::Ln:
			str += ":ln ";
			break;
		case FormulaOp::Sqrt:
			str += ":sqrt ";
			break;
		case FormulaOp::Cos:
			str += ":cos ";
			break;
		case FormulaOp::Sin:
			str += ":sin ";
			break;
		case FormulaOp::Tan:
			str += ":tan ";
			break;
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
		default:
			break;
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
