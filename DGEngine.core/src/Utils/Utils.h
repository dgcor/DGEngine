#pragma once

#include <cmath>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace Utils
{
	template <class T>
	T round(T num, unsigned int digits)
	{
		static_assert(std::is_floating_point<T>::value, "floating-point values only");
		T fac = std::pow((T)10, (T)digits);
		return std::round(num * fac) / fac;
	}

	bool endsWith(const std::string_view value, const std::string_view ending);

	std::vector<std::string> splitString(const std::string& str, const std::string& delim);

	void replaceStringInPlace(std::string& subject, const std::string_view search, const std::string_view replace);

	std::string splitInLines(std::string source, std::size_t width, std::string whitespace = " \t\r");

	std::vector<std::string> splitString(const std::string_view str, char delimiter);

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter);
	std::pair<std::string_view, std::string_view> splitStringIn2(const std::string_view str, char delimiter);

	float strtof(const std::string_view str) noexcept;
	double strtod(const std::string_view str) noexcept;
	long double strtold(const std::string_view str) noexcept;
	int strtoi(const std::string_view str) noexcept;
	long strtol(const std::string_view str) noexcept;
	long long strtoll(const std::string_view str) noexcept;
	unsigned strtou(const std::string_view str) noexcept;
	unsigned long strtoul(const std::string_view str) noexcept;
	unsigned long long strtoull(const std::string_view str) noexcept;

	template<class T>
	T strtonumber(const std::string_view str) noexcept
	{
		if constexpr (std::is_integral<T>::value == true)
		{
			if constexpr (std::is_signed<T>::value == true)
			{
				if constexpr (sizeof(T) <= 4)
				{
					return (T)strtoi(str);
				}
				else
				{
					return (T)strtoll(str);
				}
			}
			else
			{
				if constexpr (sizeof(T) <= 4)
				{
					return (T)strtou(str);
				}
				else
				{
					return (T)strtoull(str);
				}
			}
		}
		else if constexpr (std::is_floating_point<T>::value == true)
		{
			if constexpr (sizeof(T) <= 4)
			{
				return (T)strtof(str);
			}
			else
			{
				return (T)strtod(str);
			}
		}
	}

	std::string toLower(const std::string_view str);
	std::string toUpper(const std::string_view str);

	// removes trailing zeroes from doubles.
	// always returns a decimal part (ex: 125.0 instead of 125)
	std::string toString(double num);
	std::string toString(int num);
	std::string toString(long num);
	std::string toString(long long num);
	std::string toString(unsigned int num);
	std::string toString(unsigned long num);
	std::string toString(unsigned long long num);

	std::string trimStart(const std::string_view str, const std::string_view chars = " \t");
	std::string trimEnd(const std::string_view str, const std::string_view chars = " \t");
	std::string trim(const std::string_view str, const std::string_view chars = " \t");

	std::string removeEmptyLines(const std::string_view str);

	template <class T>
	long normalizeNumber(long val, const T& inputRange, const T& outputRange) noexcept
	{
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
		x = outputRange.x;
		y = outputRange.y;
		auto outputDiff = x > y ? x - y : y - x;

		val -= inputRange.x;
		val = std::lround((double)val * (double)outputDiff / (double)inputDiff) + (long)outputRange.x;
		return val;
	}
}
