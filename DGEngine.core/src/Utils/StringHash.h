#pragma once

#include <functional>
#include <string>
#include <string_view>

template <class T>
inline void hashCombine(std::size_t& s, const T& v) noexcept
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

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

template<size_t Length>
constexpr uint16_t str2int16(const char(&str)[Length]) noexcept
{
	return char2intT<uint16_t>(str, Length - 1);
}

uint16_t str2int16(const std::string& str) noexcept;

uint16_t str2int16(const std::string_view str) noexcept;

template<size_t Length>
constexpr uint32_t str2int32(const char(&str)[Length]) noexcept
{
	return char2intT<uint32_t>(str, Length - 1);
}

uint32_t str2int32(const std::string& str) noexcept;

uint32_t str2int32(const std::string_view str) noexcept;

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
