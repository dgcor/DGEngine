#pragma once

#include "LevelFlags.h"
#include "Utils/NumberVector.h"

class FlagsVector : public NumberVector<uint8_t>, public LevelFlags
{
public:
	using NumberVector::NumberVector;

	int32_t getFlags(uint32_t index, uint32_t subIndex) const override { return get(index); }
	using LevelFlags::getFlags;
};
