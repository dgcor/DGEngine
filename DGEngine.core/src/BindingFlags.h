#pragma once

#include <cstdint>
#include <type_traits>

enum class BindingFlags : int32_t
{
	OnChange = 0,
	Once = 1,
	Always = 2,
	WhenHidden = 4
};

using T = std::underlying_type_t<BindingFlags>;

constexpr BindingFlags operator~ (BindingFlags a) noexcept { return (BindingFlags)~static_cast<T>(a); }
constexpr BindingFlags operator| (BindingFlags a, BindingFlags b) noexcept { return (BindingFlags)(static_cast<T>(a) | static_cast<T>(b)); }
constexpr BindingFlags operator& (BindingFlags a, BindingFlags b) noexcept { return (BindingFlags)(static_cast<T>(a) & static_cast<T>(b)); }
constexpr BindingFlags operator^ (BindingFlags a, BindingFlags b) noexcept { return (BindingFlags)(static_cast<T>(a) ^ static_cast<T>(b)); }
constexpr BindingFlags& operator|= (BindingFlags& a, BindingFlags b) noexcept { a = (BindingFlags)(static_cast<T>(a) | static_cast<T>(b)); return a; }
constexpr BindingFlags& operator&= (BindingFlags& a, BindingFlags b) noexcept { a = (BindingFlags)(static_cast<T>(a) & static_cast<T>(b)); return a; }
constexpr BindingFlags& operator^= (BindingFlags& a, BindingFlags b) noexcept { a = (BindingFlags)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
