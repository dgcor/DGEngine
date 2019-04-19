#pragma once

#include <cmath>

// https://easings.net/
// t is the current time.
// b is the beginning value of the property.
// c is the change between the beginning and destination value of the property.
// d is the final time.
namespace EasingFunctions
{
	static constexpr auto PI = 3.14159265358979323846;

	template <class T>
	constexpr T easeLinear(T t, T b, T c, T d) noexcept
	{
		return c * (t / d) + b;
	}

	template <class T>
	constexpr T easeInQuad(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return c * t * t + b;
	}

	template <class T>
	constexpr T easeOutQuad(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return -c * t * (t - 2) + b;
	}

	template <class T>
	constexpr T easeInCubic(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return c * t * t * t + b;
	}

	template <class T>
	constexpr T easeOutCubic(T t, T b, T c, T d) noexcept
	{
		t = t / d - 1;
		return c * (t * t * t + 1) + b;
	}

	template <class T>
	constexpr T easeInQuart(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return c * t * t * t * t + b;
	}

	template <class T>
	constexpr T easeOutQuart(T t, T b, T c, T d) noexcept
	{
		t = t / d - 1;
		return -c * (t * t * t * t - 1) + b;
	}

	template <class T>
	constexpr T easeInQuint(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return c * t * t * t * t * t + b;
	}

	template <class T>
	constexpr T easeOutQuint(T t, T b, T c, T d) noexcept
	{
		t = t / d - 1;
		return c * (t * t * t * t * t + 1) + b;
	}

	template <class T>
	constexpr T easeInSine(T t, T b, T c, T d) noexcept
	{
		return -c * std::cos(t / d * (PI / 2)) + c + b;
	}

	template <class T>
	constexpr T easeOutSine(T t, T b, T c, T d) noexcept
	{
		return c * std::sin(t / d * (PI / 2)) + b;
	}

	template <class T>
	constexpr T easeInExpo(T t, T b, T c, T d) noexcept
	{
		return (t == 0) ? b : c * std::pow(2, 10 * (t / d - 1)) + b;
	}

	template <class T>
	constexpr T easeOutExpo(T t, T b, T c, T d) noexcept
	{
		return (t == d) ? b + c : c * (-std::pow(2, -10 * t / d) + 1) + b;
	}

	template <class T>
	constexpr T easeInCirc(T t, T b, T c, T d) noexcept
	{
		t /= d;
		return -c * (std::sqrt(1 - t * t) - 1) + b;
	}

	template <class T>
	constexpr T easeOutCirc(T t, T b, T c, T d) noexcept
	{
		t = t / d - 1;
		return c * std::sqrt(1 - t * t) + b;
	}
}
