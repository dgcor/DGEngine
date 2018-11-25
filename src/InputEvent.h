#pragma once

#include <type_traits>

enum class InputEvent : int
{
	None = 0,
	LeftClick = 1,
	MiddleClick = 2,
	RightClick = 4,
	MousePress = 8,
	MouseMove = 16,
	MouseRelease = 32,
	MouseScroll = 64,
	KeyPress = 128,
	TextEnter = 256,
	TouchBegin = 512,
	TouchMove = 1024,
	TouchEnd = 2048,
	All = 0xFFF
};

using T = std::underlying_type_t<InputEvent>;

constexpr InputEvent operator~ (InputEvent a) noexcept { return (InputEvent)~static_cast<T>(a); }
constexpr InputEvent operator| (InputEvent a, InputEvent b) noexcept { return (InputEvent)(static_cast<T>(a) | static_cast<T>(b)); }
constexpr InputEvent operator& (InputEvent a, InputEvent b) noexcept { return (InputEvent)(static_cast<T>(a) & static_cast<T>(b)); }
constexpr InputEvent operator^ (InputEvent a, InputEvent b) noexcept { return (InputEvent)(static_cast<T>(a) ^ static_cast<T>(b)); }
constexpr InputEvent& operator|= (InputEvent& a, InputEvent b) noexcept { a = (InputEvent)(static_cast<T>(a) | static_cast<T>(b)); return a; }
constexpr InputEvent& operator&= (InputEvent& a, InputEvent b) noexcept { a = (InputEvent)(static_cast<T>(a) & static_cast<T>(b)); return a; }
constexpr InputEvent& operator^= (InputEvent& a, InputEvent b) noexcept { a = (InputEvent)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
