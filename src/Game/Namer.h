#pragma once

#include "Queryable.h"
#include <string>
#include <vector>

struct NameValue
{
	int32_t min;
	int32_t max;
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

	std::string getName(const Queryable& obj) const;
};
