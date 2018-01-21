#pragma once

enum class IgnoreResource : int
{
	None = 0,
	Draw = 1,
	Update = 2,
	All = 4
};

using T = std::underlying_type_t<IgnoreResource>;

inline IgnoreResource operator~ (IgnoreResource a) noexcept { return (IgnoreResource)~static_cast<T>(a); }
inline IgnoreResource operator| (IgnoreResource a, IgnoreResource b) noexcept { return (IgnoreResource)(static_cast<T>(a) | static_cast<T>(b)); }
inline IgnoreResource operator& (IgnoreResource a, IgnoreResource b) noexcept { return (IgnoreResource)(static_cast<T>(a) & static_cast<T>(b)); }
inline IgnoreResource operator^ (IgnoreResource a, IgnoreResource b) noexcept { return (IgnoreResource)(static_cast<T>(a) ^ static_cast<T>(b)); }
inline IgnoreResource& operator|= (IgnoreResource& a, IgnoreResource b) noexcept { a = (IgnoreResource)(static_cast<T>(a) | static_cast<T>(b)); return a; }
inline IgnoreResource& operator&= (IgnoreResource& a, IgnoreResource b) noexcept { a = (IgnoreResource)(static_cast<T>(a) & static_cast<T>(b)); return a; }
inline IgnoreResource& operator^= (IgnoreResource& a, IgnoreResource b) noexcept { a = (IgnoreResource)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
