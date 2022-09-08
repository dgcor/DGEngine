#pragma once

#include "QueryObject.h"
#include <string_view>
#include "Utils/Number.h"
#include "Variable.h"

class Queryable
{
public:
	virtual ~Queryable() = default;

	virtual bool getNumber(const std::string_view prop, Number32& value) const;

	virtual bool getProperty(const std::string_view prop, Variable& var) const = 0;

	virtual QueryObject getQueryable(const std::string_view prop) const { return {}; }
};
