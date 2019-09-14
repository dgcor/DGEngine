#pragma once

#include "LevelFlags.h"
#include "Utils/NumberVector.h"

class FlagsVector : public NumberVector<uint8_t>, public LevelFlags
{
public:
	using NumberVector::NumberVector;

	virtual int32_t getFlags(uint32_t index, uint32_t subIndex) const { return get(index); }
	using LevelFlags::getFlags;
};
