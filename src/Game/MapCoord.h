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
};

typedef MapCoordT<uint16_t> MapCoord;
