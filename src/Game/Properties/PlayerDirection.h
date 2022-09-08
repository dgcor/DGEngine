#pragma once

#include <cstdint>
#include "Utils/PairXY.h"

enum class PlayerDirection : uint32_t
{
	Front,
	FrontLeft,
	Left,
	BackLeft,
	Back,
	BackRight,
	Right,
	FrontRight,
	All,
	Size
};

PlayerDirection getPlayerDirection(const PairFloat& currPos, const PairFloat& newPos) noexcept;
