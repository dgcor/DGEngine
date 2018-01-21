#pragma once

#include <string>
#include "Variable.h"

class Queryable
{
public:
	virtual bool getProperty(const std::string& prop, Variable& var) const = 0;
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }
};
