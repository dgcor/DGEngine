#pragma once

#include "GameProperties.h"
#include "Queryable.h"
#include <string>
#include <vector>

struct ClassifierValue
{
	typedef std::pair<LevelObjValue, LevelObjValue> ValuePair;

	// std::pair of min / max or std::string
	std::variant<ValuePair, std::string> compare;
	Variable value;
};

struct ClassifierValueInterval
{
	std::string property;
	std::vector<ClassifierValue> values;
};

class Classifier
{
private:
	std::vector<ClassifierValueInterval> classifierValues;

public:
	Classifier(std::vector<ClassifierValueInterval> values) noexcept
		: classifierValues(std::move(values)) {}

	Variable get(const Queryable& obj, uint16_t skipFirst = 0) const;
};
