#pragma once

#include <cmath>
#include <random>
#include <string>
#include <string_view>
#include <vector>

namespace Utils
{
	bool endsWith(const std::string_view value, const std::string_view ending);

	void replaceStringInPlace(std::string& subject, const std::string_view search, const std::string_view replace);

	std::string splitInLines(std::string source, std::size_t width, std::string whitespace = " \t\r");

	std::vector<std::string> splitString(const std::string_view str, char delimiter);

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter);
	std::pair<std::string_view, std::string_view> splitStringIn2(
		const std::string_view str, char delimiter);

	template <class T>
	constexpr T char2intT(const char* str, size_t length) noexcept
	{
		T hash = 5381;
		size_t i = 0;
		char c = 0;
		for (; i < length; i++)
		{
			c = str[i];
			hash = ((hash << 5) + hash) + c;
		}
		return hash;
	}

	template<class T, size_t Length>
	constexpr T str2intT(const char(&str)[Length]) noexcept
	{
		return char2intT<T>(str, Length - 1);
	}

	template<class T>
	T str2intT(const std::string& str) noexcept
	{
		return char2intT<T>(str.data(), str.size());
	}

	template<class T>
	T str2intT(const std::string_view str) noexcept
	{
		return char2intT<T>(str.data(), str.size());
	}

	float strtof(const std::string_view str) noexcept;
	double strtod(const std::string_view str) noexcept;
	long double strtold(const std::string_view str) noexcept;
	int strtoi(const std::string_view str) noexcept;
	long strtol(const std::string_view str) noexcept;
	long long strtoll(const std::string_view str) noexcept;
	unsigned strtou(const std::string_view str) noexcept;
	unsigned long strtoul(const std::string_view str) noexcept;
	unsigned long long strtoull(const std::string_view str) noexcept;

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

	class Random
	{
	private:
		static std::random_device rd;
		static std::mt19937 mt;

	public:
		template <class T>
		static T get(T max)
		{
			std::uniform_int_distribution<T> dist(0, max);
			return dist(mt);
		}

		template <class T>
		static T get(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(mt);
		}

		template <class T>
		static T getf()
		{
			std::uniform_real_distribution<T> dist(0., 1.);
			return dist(mt);
		}

		template <class T>
		static T getf(T max)
		{
			std::uniform_real_distribution<T> dist(0., max);
			return dist(mt);
		}

		template <class T>
		static T getf(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(mt);
		}
	};
}

#define str2int16 Utils::str2intT<uint16_t>
#define str2int32 Utils::str2intT<uint32_t>
