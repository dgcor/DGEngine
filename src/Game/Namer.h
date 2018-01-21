#pragma once

#include "GameProperties.h"
#include "Queryable.h"
#include <string>
#include <vector>

struct NameValue
{
	LevelObjValue min{ 0 };
	LevelObjValue max{ 0 };
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
	Namer(std::vector<NameValueInterval> nameValues_) noexcept
		: nameValues(std::move(nameValues_)) {}

	std::string getName(const Queryable& obj, uint16_t skipFirst = 0) const;
};
