#pragma once

#include <cstdint>

enum class PlayerStatus : uint32_t
{
	Stand,
	Walk,
	Attack,
	Dead,
	Size
};
