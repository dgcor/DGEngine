#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

struct StringViewHashEq
{
	using is_transparent = void;
	using hash_type = std::hash<std::string_view>;
	using transparent_key_equal = std::equal_to<>;
	size_t operator()(std::string_view str) const { return hash_type{}(str); }
	size_t operator()(const std::string& str) const { return hash_type{}(str); }
	size_t operator()(const char* str) const { return hash_type{}(str); }

	size_t operator()(std::string_view str1, std::string_view str2) const { return transparent_key_equal{}(str1, str2); }
	size_t operator()(const std::string& str1, const std::string& str2) const { return transparent_key_equal{}(str1, str2); }
	size_t operator()(const char* str1, const char* str2) const { return transparent_key_equal{}(str1, str2); }
};

// std::unordered_map<std::string, T> with std::string_view support
template <class T>
using UnorderedStringMap = std::unordered_map<std::string, T, StringViewHashEq, StringViewHashEq>;

// std::unordered_multimap<std::string, T> with std::string_view support
template <class T>
using UnorderedStringMultiMap = std::unordered_multimap<std::string, T, StringViewHashEq, StringViewHashEq>;
