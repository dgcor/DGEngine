#pragma once

#include "Game/Properties/LevelObjValue.h"
#include "Game/Queryable.h"
#include "Game/TextureQueryable.h"
#include "Utils/Number.h"

class LevelObjectQueryable : public Queryable, public TextureQueryable
{
public:
	virtual bool getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const = 0;

	virtual bool setNumberByHash(uint16_t propHash, const Number32& value) { return false; }

	virtual bool setProperty(const std::string_view prop, const Variable& var);
};
