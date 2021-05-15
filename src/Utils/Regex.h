#pragma once

#include "re.h"
#include <string>
#include <string_view>

class Regex
{
private:
	std::string pattern;
	re_t regex = {};

public:
	Regex() {}
	Regex(const std::string_view pattern_) : pattern(pattern_)
	{
		regex = re_compile(pattern.c_str());
	}

	bool isValid() const noexcept { return regex != nullptr; }

	bool match(const std::string_view str) const noexcept
	{
		int matchedLength = 0;
		int isMatch = re_matchp(regex, str.data(), &matchedLength);
		return isMatch != -1 && (size_t)matchedLength == str.length();
	}

	static bool match(const char* pattern, const std::string_view str) noexcept
	{
		int matchedLength = 0;
		int isMatch = re_match(pattern, str.data(), &matchedLength);
		return isMatch != -1 && (size_t)matchedLength == str.length();
	}
};
