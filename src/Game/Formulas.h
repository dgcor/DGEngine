#pragma once

#include "Formula.h"

template <class FormulaContainer>
class Formulas : public FormulaContainer
{
public:
	template <class NumberType>
	NumberType eval(size_t idx,
		const Queryable& query, NumberType default_ = {}) const
	{
		if ((*this)[idx].empty() == true)
		{
			return default_;
		}
		return (NumberType)(*this)[idx].eval(query);
	}

	template <class NumberType>
	NumberType eval(size_t idx, const Queryable& queryA,
		const Queryable& queryB, NumberType default_ = {}) const
	{
		if ((*this)[idx].empty() == true)
		{
			return default_;
		}
		return (NumberType)(*this)[idx].eval(queryA, queryB);
	}

	template <class NumberType>
	NumberType eval(size_t idx, const Queryable& queryA,
		const Queryable& queryB, const std::string_view minMaxNumber) const
	{
		if ((*this)[idx].empty() == true)
		{
			return {};
		}
		return (NumberType)(*this)[idx].eval(queryA, queryB, minMaxNumber);
	}

	template <class NumberType>
	NumberType evalMin(size_t idx, const Queryable& queryA,
		const Queryable& queryB, NumberType default_ = {}) const
	{
		if ((*this)[idx].empty() == true)
		{
			return default_;
		}
		return (NumberType)(*this)[idx].eval(queryA, queryB, -2);
	}

	template <class NumberType>
	NumberType evalMax(size_t idx, const Queryable& queryA,
		const Queryable& queryB, NumberType default_ = {}) const
	{
		if ((*this)[idx].empty() == true)
		{
			return default_;
		}
		return (NumberType)(*this)[idx].eval(queryA, queryB, -1);
	}
};
