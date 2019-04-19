#pragma once

#include <cstdint>

template <class T>
struct PairXY
{
	T x;
	T y;

	constexpr PairXY() noexcept: x(0), y(0) {}
	constexpr PairXY(T x_, T y_) noexcept : x(x_), y(y_) {}

	bool operator==(const PairXY& rhs) const noexcept
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const PairXY& rhs) const noexcept
	{
		return !operator==(rhs);
	}
};

typedef PairXY<float> PairFloat;
typedef PairXY<int8_t> PairInt8;
typedef PairXY<int16_t> PairInt16;
typedef PairXY<int32_t> PairInt32;
typedef PairXY<uint8_t> PairUInt8;
typedef PairXY<uint16_t> PairUInt16;
typedef PairXY<uint32_t> PairUInt32;
