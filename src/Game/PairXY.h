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
typedef PairXY<int32_t> PairInt;
