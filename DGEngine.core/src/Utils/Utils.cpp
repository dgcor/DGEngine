#include "Utils.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <sstream>

namespace Utils
{
	std::string removeEmptyLines(const std::string_view str)
	{
		auto ret = trim(str, "\n");
		ret.erase(std::unique(ret.begin(), ret.end(),
			[](char a, char b) { return a == '\n' && b == '\n'; }),
			ret.end());
		return ret;
	}

	void replaceStringInPlace(std::string& subject, const std::string_view search, const std::string_view replace)
	{
		if (search.empty() == true)
		{
			return;
		}
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	std::vector<std::string_view> splitString(const std::string_view str, const std::string_view delimiters)
	{
		std::vector<std::string_view> tokens;
		auto start = str.find_first_not_of(delimiters, 0);
		auto stop = str.find_first_of(delimiters, start);

		while (stop != std::string_view::npos || start != std::string_view::npos)
		{
			tokens.push_back(str.substr(start, stop - start));
			start = str.find_first_not_of(delimiters, stop);
			stop = str.find_first_of(delimiters, start);
		}
		return tokens;
	}

	std::pair<std::string, std::string> splitStringIn2(const std::string& str, char delimiter)
	{
		auto pos = str.find(delimiter, 0);
		if (pos != std::string::npos)
		{
			return { str.substr(0, pos), str.substr(pos + 1, str.size() - pos) };
		}
		return { str, "" };
	}

	std::pair<std::string_view, std::string_view> splitStringIn2(const std::string_view str, char delimiter)
	{
		auto pos = str.find(delimiter, 0);
		if (pos != std::string_view::npos)
		{
			return { str.substr(0, pos), str.substr(pos + 1, str.size() - pos) };
		}
		return { str, "" };
	}

	float strtof(std::string_view str, float defaultVal) noexcept
	{
		return strToNumber<float>(str, defaultVal);
	}

	double strtod(std::string_view str, double defaultVal) noexcept
	{
		return strToNumber<double>(str, defaultVal);
	}

	long double strtold(std::string_view str, long double defaultVal) noexcept
	{
		return strToNumber<long double>(str, defaultVal);
	}

	int strtoi(std::string_view str, int defaultVal) noexcept
	{
		return strToNumber<int>(str, defaultVal);
	}

	long strtol(std::string_view str, long defaultVal) noexcept
	{
		return strToNumber<long>(str, defaultVal);
	}

	long long strtoll(std::string_view str, long long defaultVal) noexcept
	{
		return strToNumber<long long>(str, defaultVal);
	}

	unsigned strtou(std::string_view str, unsigned defaultVal) noexcept
	{
		return strToNumber<unsigned>(str, defaultVal);
	}

	unsigned long strtoul(std::string_view str, unsigned long defaultVal) noexcept
	{
		return strToNumber<unsigned long>(str, defaultVal);
	}

	unsigned long long strtoull(std::string_view str, unsigned long long defaultVal) noexcept
	{
		return strToNumber<unsigned long long>(str, defaultVal);
	}

	std::string toLower(const std::string_view str)
	{
		std::string ret(str);
		std::transform(ret.begin(), ret.end(), ret.begin(), static_cast<int(*)(int)>(std::tolower));
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

	std::string toUpper(const std::string_view str)
	{
		std::string ret(str);
		std::transform(ret.begin(), ret.end(), ret.begin(), static_cast<int(*)(int)>(std::toupper));
		return ret;
	}

	std::string trim(const std::string_view str, const std::string_view chars)
	{
		auto begin = str.find_first_not_of(chars);
		if (begin == std::string::npos)
		{
			return {};
		}

		auto end = str.find_last_not_of(chars);
		auto range = end - begin + 1;
		return std::string(str.substr(begin, range));
	}

	std::string trimEnd(const std::string_view str, const std::string_view chars)
	{
		std::string ret(str);
		ret.erase(ret.find_last_not_of(chars) + 1);
		return ret;
	}

	std::string trimStart(const std::string_view str, const std::string_view chars)
	{
		std::string ret(str);
		ret.erase(0, ret.find_first_not_of(chars));
		return ret;
	}

	std::string wordWrap(std::string str, int length)
	{
		if (length < 0)
		{
			return str;
		}

		auto it = str.begin();
		auto itEnd = str.end();
		auto lastSpaceIdx = str.find_last_not_of(' ');
		if (lastSpaceIdx != std::string::npos)
		{
			itEnd = it + lastSpaceIdx + 1;
		}

		auto strLength = itEnd - it;

		if (strLength <= length)
		{
			return str;
		}

		auto itLastSpace = str.begin();
		int distanceToWidth = 0;

		while (it != itEnd)
		{
			while (it != itEnd && distanceToWidth <= length)
			{
				distanceToWidth++;

				if (*it == ' ')
				{
					itLastSpace = it;

					if (distanceToWidth == length)
					{
						if (it + 1 != itEnd)
						{
							*itLastSpace = '\n';
							distanceToWidth = 0;
						}
					}
				}
				++it;
			}

			if (itLastSpace != str.begin())
			{
				if (distanceToWidth != length || it != itEnd)
				{
					*itLastSpace = '\n';
				}
			}

			distanceToWidth = std::clamp((int)(it - itLastSpace) - 2, 0, length);
			itLastSpace = str.begin();
		}

		return str;
	}
}
