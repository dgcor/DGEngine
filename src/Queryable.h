#pragma once

#include <string_view>
#include "TextureInfo.h"
#include "Variable.h"

class Queryable
{
public:
	virtual ~Queryable() = default;
	virtual bool getProperty(const std::string_view prop, Variable& var) const = 0;
	virtual const Queryable* getQueryable(const std::string_view prop) const { return nullptr; }
	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const { return false; }
};
