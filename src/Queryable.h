#pragma once

#include "Game/Number.h"
#include <string_view>
#include "TextureInfo.h"
#include "Variable.h"

class Queryable
{
public:
	virtual ~Queryable() = default;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const { return false; }

	virtual bool getProperty(const std::string_view prop, Variable& var) const = 0;

	virtual const Queryable* getQueryable(const std::string_view prop) const { return nullptr; }

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const { return false; }
};
