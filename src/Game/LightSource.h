#pragma once

#include <cstdint>

// easing function to apply to light (ease in functions).
enum class LightEasing : uint8_t
{
	Linear,
	Sine,
	Quad,
	Cubic,
	Quart,
	Quint,
	Expo,
	Circ
};

struct LightSource
{
	uint8_t minLight{ 0 };
	uint8_t maxLight{ 0 };
	uint8_t radius{ 0 };
	LightEasing easing{ LightEasing::Linear };
};
