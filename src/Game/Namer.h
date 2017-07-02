#pragma once

#include "GameProperties.h"
#include "Queryable.h"
#include <string>
#include <vector>

struct NameValue
{
	LevelObjValue min;
	LevelObjValue max;
	std::string text;
};

struct NameValueInterval
{
	std::string property;
	std::vector<NameValue> values;
};

class Namer
{
private:
	std::vector<NameValueInterval> nameValues;

public:
	Namer(const std::vector<NameValueInterval>& nameValues_) : nameValues(nameValues_) {}

	std::string getName(const Queryable& obj, uint16_t skipFirst = 0) const;
};
