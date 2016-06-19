#pragma once

#include <type_traits>

enum class Anchor : int
{
	None = 0,
	Top = 1,
	Bottom = 2,
	Left = 4,
	Right = 8
};

using T = std::underlying_type_t<Anchor>;

inline Anchor operator~ (Anchor a) { return (Anchor)~static_cast<T>(a); }
inline Anchor operator| (Anchor a, Anchor b) { return (Anchor)(static_cast<T>(a) | static_cast<T>(b)); }
inline Anchor operator& (Anchor a, Anchor b) { return (Anchor)(static_cast<T>(a) & static_cast<T>(b)); }
inline Anchor operator^ (Anchor a, Anchor b) { return (Anchor)(static_cast<T>(a) ^ static_cast<T>(b)); }
inline Anchor& operator|= (Anchor& a, Anchor b) { a = (Anchor)(static_cast<T>(a) | static_cast<T>(b)); return a; }
inline Anchor& operator&= (Anchor& a, Anchor b) { a = (Anchor)(static_cast<T>(a) & static_cast<T>(b)); return a; }
inline Anchor& operator^= (Anchor& a, Anchor b) { a = (Anchor)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
