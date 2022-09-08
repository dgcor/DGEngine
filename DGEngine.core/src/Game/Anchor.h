#pragma once

#include <cstdint>
#include <type_traits>

enum class Anchor : int32_t
{
	None = 0,
	Top = 1,
	Bottom = 2,
	Left = 4,
	Right = 8,
	All = 0xF
};

using T = std::underlying_type_t<Anchor>;

constexpr Anchor operator~ (Anchor a) noexcept { return (Anchor)~static_cast<T>(a); }
constexpr Anchor operator| (Anchor a, Anchor b) noexcept { return (Anchor)(static_cast<T>(a) | static_cast<T>(b)); }
constexpr Anchor operator& (Anchor a, Anchor b) noexcept { return (Anchor)(static_cast<T>(a) & static_cast<T>(b)); }
constexpr Anchor operator^ (Anchor a, Anchor b) noexcept { return (Anchor)(static_cast<T>(a) ^ static_cast<T>(b)); }
constexpr Anchor& operator|= (Anchor& a, Anchor b) noexcept { a = (Anchor)(static_cast<T>(a) | static_cast<T>(b)); return a; }
constexpr Anchor& operator&= (Anchor& a, Anchor b) noexcept { a = (Anchor)(static_cast<T>(a) & static_cast<T>(b)); return a; }
constexpr Anchor& operator^= (Anchor& a, Anchor b) noexcept { a = (Anchor)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
