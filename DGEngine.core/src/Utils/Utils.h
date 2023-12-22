#pragma once

#include <cassert>
#include <charconv>
#include <cmath>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Utils
{
	template <class T>
	long normalizeNumber(long val, const T& inputRange, const T& outputRange) noexcept
	{
		assert(inputRange.x <= inputRange.y);
		assert(outputRange.x <= outputRange.y);

		if (val < (long)inputRange.x)
		{
			val = (long)inputRange.x;
		}
		else if (val > (long)inputRange.y)
		{
			val = (long)inputRange.y;
		}

		auto x = inputRange.x;
		auto y = inputRange.y;
		auto inputDiff = x > y ? x - y : y - x;
		if ((long)inputDiff == 0)
		{
			return (long)outputRange.x;
		}

		x = outputRange.x;
		y = outputRange.y;
		auto outputDiff = x > y ? x - y : y - x;

		val -= (long)inputRange.x;
		val = std::lround((double)val * (double)outputDiff / (double)inputDiff) + (long)outputRange.x;
		return val;
	}

	std::string removeEmptyLines(const std::string_view str);

	void replaceStringInPlace(std::string& subject, const std::string_view search, const std::string_view replace);

	template <class T>
	T round(T num, unsigned int digits)
	{
		static_assert(std::is_floating_point_v<T>, "floating-point values only");
		T fac = std::pow((T)10, (T)digits);
		return std::round(num * fac) / fac;
	}

	std::vector<std::string_view> splitString(const std::string_view str, const std::string_view delimiters);

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter);
	std::pair<std::string_view, std::string_view> splitStringIn2(const std::string_view str, char delimiter);

	float strtof(const std::string_view str, float defaultVal = {}) noexcept;
	double strtod(const std::string_view str, double defaultVal = {}) noexcept;
	long double strtold(const std::string_view str, long double defaultVal = {}) noexcept;
	int strtoi(const std::string_view str, int defaultVal = {}) noexcept;
	long strtol(const std::string_view str, long defaultVal = {}) noexcept;
	long long strtoll(const std::string_view str, long long defaultVal = {}) noexcept;
	unsigned strtou(const std::string_view str, unsigned defaultVal = {}) noexcept;
	unsigned long strtoul(const std::string_view str, unsigned long defaultVal = {}) noexcept;
	unsigned long long strtoull(const std::string_view str, unsigned long long defaultVal = {}) noexcept;

	template<class T>
	T strToNumber(const std::string_view str, T defaultVal = {}) noexcept
	{
		T val = {};
		auto err = std::from_chars(str.data(), str.data() + str.size(), val);
		if (err.ec == std::errc() ||
			err.ec == std::errc::result_out_of_range)
		{
			return val;
		}
		return defaultVal;
	}

	template<class T>
	std::optional<T> strToNumberOpt(const std::string_view str) noexcept
	{
		T val = {};
		auto err = std::from_chars(str.data(), str.data() + str.size(), val);
		if (err.ec == std::errc())
		{
			return val;
		}
		return {};
	}

	std::string toLower(const std::string_view str);

	// removes trailing zeroes from doubles.
	// always returns a decimal part (ex: 125.0 instead of 125)
	std::string toString(double num);
	std::string toString(int num);
	std::string toString(long num);
	std::string toString(long long num);
	std::string toString(unsigned int num);
	std::string toString(unsigned long num);
	std::string toString(unsigned long long num);

	std::string toUpper(const std::string_view str);

	std::string trim(const std::string_view str, const std::string_view chars = " \t");
	std::string trimEnd(const std::string_view str, const std::string_view chars = " \t");
	std::string trimStart(const std::string_view str, const std::string_view chars = " \t");

	std::string wordWrap(std::string str, int length);
}
