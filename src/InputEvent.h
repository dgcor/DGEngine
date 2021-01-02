#pragma once

#include <compare>
#include <cstdint>
#include <SFML/Window/Event.hpp>
#include <type_traits>

struct CompareEvent
{
	static bool compare(const sf::Event& obj1, const sf::Event& obj2) noexcept;

	size_t operator()(const sf::Event& obj) const noexcept;
	bool operator()(const sf::Event& obj1, const sf::Event& obj2) const noexcept;
};

enum class InputType : int32_t
{
	Mouse,
	Keyboard,
	Joystick
};

struct InputEvent
{
	InputType type;
	int32_t value;

	bool isActive() const;

	auto operator<=>(const InputEvent&) const = default;
};

enum class InputEventType : int32_t
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

using T = std::underlying_type_t<InputEventType>;

constexpr InputEventType operator~ (InputEventType a) noexcept { return (InputEventType)~static_cast<T>(a); }
constexpr InputEventType operator| (InputEventType a, InputEventType b) noexcept { return (InputEventType)(static_cast<T>(a) | static_cast<T>(b)); }
constexpr InputEventType operator& (InputEventType a, InputEventType b) noexcept { return (InputEventType)(static_cast<T>(a) & static_cast<T>(b)); }
constexpr InputEventType operator^ (InputEventType a, InputEventType b) noexcept { return (InputEventType)(static_cast<T>(a) ^ static_cast<T>(b)); }
constexpr InputEventType& operator|= (InputEventType& a, InputEventType b) noexcept { a = (InputEventType)(static_cast<T>(a) | static_cast<T>(b)); return a; }
constexpr InputEventType& operator&= (InputEventType& a, InputEventType b) noexcept { a = (InputEventType)(static_cast<T>(a) & static_cast<T>(b)); return a; }
constexpr InputEventType& operator^= (InputEventType& a, InputEventType b) noexcept { a = (InputEventType)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }
