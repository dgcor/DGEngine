#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>

namespace Utils
{
	std::random_device Random::rd;
	std::mt19937 Random::mt(rd());

	bool endsWith(const std::string& value, const std::string& ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	std::string splitInLines(std::string source, std::size_t width, std::string whitespace)
	{
		std::size_t  currIndex = width - 1;
		std::size_t  sizeToElim;
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
			sizeToElim = source.find_first_not_of(whitespace, currIndex + 1) - currIndex - 1;
			source.replace(currIndex + 1, sizeToElim, "\n");
			currIndex += (width + 1); //due to the recently inserted "\n"
		}
		return source;
	}

	void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	std::vector<std::string> getStringVector(const std::string& regexStr, const std::string& str)
	{
		std::regex reg(regexStr, std::regex_constants::ECMAScript);
		return std::vector<std::string>(std::sregex_token_iterator(str.begin(), str.end(), reg), std::sregex_token_iterator());
	}

	std::vector<std::string> splitString(const std::string& str, char delimiter)
	{
		std::vector<std::string> strings;

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		strings.push_back(str.substr(prev));

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

	std::string toLower(const std::string& str)
	{
		auto ret = str;
		std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
		return ret;
	}

	std::string toUpper(const std::string& str)
	{
		auto ret = str;
		std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
		return ret;
	}

	std::string toString(double d)
	{
		std::string str{ std::to_string(d) };
		int offset{ 1 };
		if (str.find_last_not_of('0') == str.find('.'))
		{
			offset = 2;
		}
		str.erase(str.find_last_not_of('0') + offset, std::string::npos);
		return str;
	}

	std::string trimStart(const std::string& str, const std::string& chars)
	{
		auto startPos = str.find_first_not_of(chars);
		if (startPos != std::string::npos)
		{
			return str.substr(startPos);
		}
		return str;
	}

	std::string trimEnd(const std::string& str, const std::string& chars)
	{
		auto endPos = str.find_last_not_of(chars);
		if (endPos != std::string::npos)
		{
			return str.substr(0, endPos + 1);
		}
		return str;
	}

	std::string trim(const std::string& str, const std::string& chars)
	{
		return trimStart(trimEnd(str, chars), chars);
	}

	std::string removeEmptyLines(const std::string& str)
	{
		auto ret = trim(str, "\n");
		ret.erase(std::unique(ret.begin(), ret.end(),
			[](char a, char b) { return a == '\n' && b == '\n'; }),
			ret.end());
		return ret;
	}
}
