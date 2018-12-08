#pragma once

#include "Utils/NumberVector.h"

class Sol : public NumberVector<uint8_t>
{
public:
	using NumberVector<uint8_t>::NumberVector;

	bool passable(size_t index) const;
};
