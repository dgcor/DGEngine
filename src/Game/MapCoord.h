#pragma once

#include <cstdint>
#include <utility>

template <class T>
struct MapCoordT
{
	T x;
	T y;

	MapCoordT() : x(0), y(0) {}
	MapCoordT(T x_, T y_) : x(x_), y(y_) {}

	bool operator==(const MapCoordT& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const MapCoordT& rhs) const
	{
		return !operator==(rhs);
	}
};

typedef MapCoordT<uint16_t> MapCoord;
