#pragma once

#include <cstdint>
#include <type_traits>

enum class AnimationFlags : int32_t
{
	Invalid = 0,
	Valid = 1,
	Overflow = 2
};

using T = std::underlying_type_t<AnimationFlags>;

constexpr AnimationFlags operator~ (AnimationFlags a) noexcept { return (AnimationFlags)~static_cast<T>(a); }
constexpr AnimationFlags operator| (AnimationFlags a, AnimationFlags b) noexcept { return (AnimationFlags)(static_cast<T>(a) | static_cast<T>(b)); }
constexpr AnimationFlags operator& (AnimationFlags a, AnimationFlags b) noexcept { return (AnimationFlags)(static_cast<T>(a) & static_cast<T>(b)); }
constexpr AnimationFlags operator^ (AnimationFlags a, AnimationFlags b) noexcept { return (AnimationFlags)(static_cast<T>(a) ^ static_cast<T>(b)); }
constexpr AnimationFlags& operator|= (AnimationFlags& a, AnimationFlags b) noexcept { a = (AnimationFlags)(static_cast<T>(a) | static_cast<T>(b)); return a; }
constexpr AnimationFlags& operator&= (AnimationFlags& a, AnimationFlags b) noexcept { a = (AnimationFlags)(static_cast<T>(a) & static_cast<T>(b)); return a; }
constexpr AnimationFlags& operator^= (AnimationFlags& a, AnimationFlags b) noexcept { a = (AnimationFlags)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
