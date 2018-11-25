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

	bool getText(size_t idx, const Queryable& obj, std::string& text) const
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
		text = VarUtils::toString(classifier->get(obj, classifiers[idx].second));
		if (text.empty() == false)
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
