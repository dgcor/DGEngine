#pragma once

#include <array>
#include "Classifier.h"
#include "GameUtils.h"

class Queryable;

template <size_t Size_>
class Classifiers
{
private:
	std::array<std::pair<Classifier*, uint16_t>, Size_> classifiers;

public:
	bool getVar(size_t idx, const Queryable& obj, Variable& var) const
	{
		if (idx >= classifiers.size())
		{
			return false;
		}
		auto classifier = classifiers[idx].first;
		if (classifier == nullptr)
		{
			return false;
		}
		var = classifier->get(obj, classifiers[idx].second);
		return true;
	}

	template <class T>
	T getNumber(size_t idx, const Queryable& obj) const
	{
		Variable var;
		if (getVar(idx, obj, var) == false)
		{
			return false;
		}
		if (std::holds_alternative<int64_t>(var) == true)
		{
			return std::get<int64_t>(var);
		}
		else if (std::holds_alternative<double>(var) == true)
		{
			return std::get<double>(var);
		}
		return {};
	}

	bool getText(size_t idx, const Queryable& obj,
		std::string& text, bool replaceVars = true) const
	{
		Variable var;
		if (getVar(idx, obj, var) == false)
		{
			return false;
		}
		text = VarUtils::toString(var);
		if (replaceVars == true && text.empty() == false)
		{
			text = GameUtils::replaceStringWithQueryable(text, obj);
		}
		return true;
	}

	void set(size_t idx, Classifier* classifier, uint16_t skipFirst)
	{
		if (idx >= classifiers.size())
		{
			return;
		}
		classifiers[idx] = std::make_pair(classifier, skipFirst);
	}
};
