#pragma once

#include <cstdint>

template <class T>
struct PairXY
{
	T x;
	T y;

	constexpr PairXY() : x(0), y(0) {}
	constexpr PairXY(T x_, T y_) : x(x_), y(y_) {}

	bool operator==(const PairXY& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const PairXY& rhs) const
	{
		return !operator==(rhs);
	}
};

typedef PairXY<float> PairFloat;
typedef PairXY<int32_t> PairInt;
