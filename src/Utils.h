#pragma once

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

#if (_MSC_VER == 1900)
	template <class T>
	constexpr T str2intT(const char* str, T h = 0)
	{
		return !str[h] ? 5381 : (str2intT(str, h + 1) * 33) ^ str[h];
	}
#else
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
#endif

	std::string toLower(const std::string& str);
	std::string toUpper(const std::string& str);

	std::string trimStart(const std::string& str, const std::string& chars = " \t");
	std::string trimEnd(const std::string& str, const std::string& chars = " \t");
	std::string trim(const std::string& str, const std::string& chars = " \t");

	std::string removeEmptyLines(const std::string& str);

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
