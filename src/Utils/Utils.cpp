#include "Utils.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>
#include <iterator>
#include <sstream>

namespace Utils
{
	std::random_device RandomGenerator::rd;
	std::mt19937 RandomGenerator::generator(rd());

	bool endsWith(const std::string_view value, const std::string_view ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	std::string splitInLines(std::string source, std::size_t width, std::string whitespace)
	{
		std::size_t  currIndex = width - 1;
		while (currIndex < source.length())
		{
			currIndex = source.find_last_of(whitespace, currIndex + 1);
			if (currIndex == std::string::npos)
			{
				break;
			}
			currIndex = source.find_last_not_of(whitespace, currIndex);
			if (currIndex == std::string::npos)
			{
				break;
			}
			auto sizeToElim = source.find_first_not_of(whitespace, currIndex + 1) - currIndex - 1;
			source.replace(currIndex + 1, sizeToElim, "\n");
			currIndex += (width + 1); //due to the recently inserted "\n"
		}
		return source;
	}

	std::vector<std::string> splitString(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos)
			{
				pos = str.length();
			}
			std::string token = str.substr(prev, pos - prev);
			if (token.empty() == false)
			{
				tokens.push_back(token);
			}
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}

	void replaceStringInPlace(std::string& subject, const std::string_view search, const std::string_view replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	std::vector<std::string> splitString(const std::string_view str, char delimiter)
	{
		std::vector<std::string> strings;

		std::string_view::size_type pos = 0;
		std::string_view::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(std::string(str.substr(prev, pos - prev)));
			prev = pos + 1;
		}

		strings.push_back(std::string(str.substr(prev)));

		return strings;
	}

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter)
	{
		auto pos = str.find(delimiter, 0);
		if (pos != std::string::npos)
		{
			return std::make_pair(str.substr(0, pos), str.substr(pos + 1, str.size() - pos));
		}
		return std::make_pair(str, "");
	}

	std::pair<std::string_view, std::string_view> splitStringIn2(
		const std::string_view str, char delimiter)
	{
		auto pos = str.find(delimiter, 0);
		if (pos != std::string::npos)
		{
			return std::make_pair(str.substr(0, pos), str.substr(pos + 1, str.size() - pos));
		}
		return std::make_pair(str, "");
	}

	float strtof(std::string_view str) noexcept
	{
		float val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	double strtod(std::string_view str) noexcept
	{
		double val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	long double strtold(std::string_view str) noexcept
	{
		long double val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	int strtoi(std::string_view str) noexcept
	{
		int val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	long strtol(std::string_view str) noexcept
	{
		long val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	long long strtoll(std::string_view str) noexcept
	{
		long long val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	unsigned strtou(std::string_view str) noexcept
	{
		unsigned val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	unsigned long strtoul(std::string_view str) noexcept
	{
		unsigned long val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	unsigned long long strtoull(std::string_view str) noexcept
	{
		unsigned long long val = 0;
		std::from_chars(str.data(), str.data() + str.size(), val);
		return val;
	}

	std::string toLower(const std::string_view str)
	{
		std::string ret(str);
		std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
		return ret;
	}

	std::string toUpper(const std::string_view str)
	{
		std::string ret(str);
		std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
		return ret;
	}

	std::string toString(double num)
	{
		std::array<char, 24> str;
		auto err = std::to_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc())
		{
			std::string ret(str.data(), err.ptr - str.data());
			if (ret.find('.') == std::string::npos)
			{
				ret += ".0";
			}
			return ret;
		}
		return { "0.0" };
	}

	std::string toString(int num)
	{
		if constexpr (sizeof(int) == 8)
		{
			return toString((long long)num);
		}
		std::array<char, 11> str;
		auto err = std::to_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc())
		{
			return std::string(str.data(), err.ptr - str.data());
		}
		return { "0" };
	}

	std::string toString(long num)
	{
		if constexpr (sizeof(long) <= 4)
		{
			return toString((int)num);
		}
		else
		{
			return toString((long long)num);
		}
	}

	std::string toString(long long num)
	{
		if constexpr (sizeof(long long) == 4)
		{
			return toString((int)num);
		}
		std::array<char, 21> str;
		auto err = std::to_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc())
		{
			auto str2 = std::string(str.data(), err.ptr - str.data());
			return str2;
		}
		return { "0" };
	}

	std::string toString(unsigned int num)
	{
		if constexpr (sizeof(unsigned int) == 8)
		{
			return toString((unsigned long long)num);
		}
		std::array<char, 11> str;
		auto err = std::to_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc())
		{
			return std::string(str.data(), err.ptr - str.data());
		}
		return { "0" };
	}

	std::string toString(unsigned long num)
	{
		if constexpr (sizeof(unsigned long) <= 4)
		{
			return toString((unsigned int)num);
		}
		else
		{
			return toString((unsigned long long)num);
		}
	}

	std::string toString(unsigned long long num)
	{
		if constexpr (sizeof(unsigned long long) == 4)
		{
			return toString((unsigned int)num);
		}
		std::array<char, 21> str;
		auto err = std::to_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc())
		{
			return std::string(str.data(), err.ptr - str.data());
		}
		return { "0" };
	}

	std::string trimStart(const std::string_view str, const std::string_view chars)
	{
		std::string ret(str);
		ret.erase(0, ret.find_first_not_of(chars));
		return ret;
	}

	std::string trimEnd(const std::string_view str, const std::string_view chars)
	{
		std::string ret(str);
		ret.erase(ret.find_last_not_of(chars) + 1);
		return ret;
	}

	std::string trim(const std::string_view str, const std::string_view chars)
	{
		return trimStart(trimEnd(str, chars), chars);
	}

	std::string removeEmptyLines(const std::string_view str)
	{
		auto ret = trim(str, "\n");
		ret.erase(std::unique(ret.begin(), ret.end(),
			[](char a, char b) { return a == '\n' && b == '\n'; }),
			ret.end());
		return ret;
	}
}
