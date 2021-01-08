#pragma once

#include <cstdint>
#include <string_view>
#include "Utils/Vector2D.h"

class Dun : public Vector2D<int32_t>
{
public:
	using Vector2D::Vector2D;
	Dun(const std::string_view fileName, int32_t defaultTile = -1);
};
