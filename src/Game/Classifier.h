#pragma once

#include "Game/Properties/LevelObjValue.h"
#include "Game/Queryable.h"
#include <string>
#include <vector>

struct ClassifierValue
{
	using ValuePair = std::pair<LevelObjValue, LevelObjValue>;

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
