#pragma once

#include <string_view>
#include "Utils/Number.h"
#include "Variable.h"

struct TextureInfo;

class Queryable
{
public:
	virtual ~Queryable() = default;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const
	{
		Variable var;
		if (getProperty(prop, var) == true)
		{
			if (std::holds_alternative<int64_t>(var) == true)
			{
				value.setInt32((int32_t)std::get<int64_t>(var));
				return true;
			}
			else if (std::holds_alternative<double>(var) == true)
			{
				value.setFloat((float)std::get<double>(var));
				return true;
			}
			else if (std::holds_alternative<bool>(var) == true)
			{
				value.setInt32(std::get<bool>(var) ? 1 : 0);
				return true;
			}
		}
		return false;
	}

	virtual bool getProperty(const std::string_view prop, Variable& var) const = 0;

	virtual const Queryable* getQueryable(const std::string_view prop) const { return nullptr; }

	virtual bool getTexture(uint32_t textureNumber, TextureInfo& ti) const { return false; }
};
