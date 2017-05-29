#pragma once

#include <cmath>
#include <random>
#include <string>
#include <vector>

namespace Utils
{
	bool endsWith(const std::string& value, const std::string& ending);

	std::vector<std::string> getStringVector(const std::string& regexStr, const std::string& str);

	void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

	std::string splitInLines(std::string source, std::size_t width, std::string whitespace = " \t\r");

	std::vector<std::string> splitString(const std::string& str, char delimiter);

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter);

	template <class T>
	constexpr T str2intT(const char* str)
	{
		T hash = 5381;
		char c = 0;
		while ((c = *str++) != 0)
		{
			hash = ((hash << 5) + hash) + c;
		}
		return hash;
	}

	std::string toLower(const std::string& str);
	std::string toUpper(const std::string& str);

	// removes trailing zeroes from doubles
	std::string toString(double d);

	std::string trimStart(const std::string& str, const std::string& chars = " \t");
	std::string trimEnd(const std::string& str, const std::string& chars = " \t");
	std::string trim(const std::string& str, const std::string& chars = " \t");

	std::string removeEmptyLines(const std::string& str);

	template <class T>
	long normalizeNumber(long val, const T& inputRange, const T& outputRange)
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
		val = std::lround((double)val * (double)outputDiff / (double)inputDiff) + (double)outputRange.x;
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
