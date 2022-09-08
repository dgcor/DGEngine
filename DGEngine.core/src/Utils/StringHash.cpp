#include "StringHash.h"

uint16_t str2int16(const std::string& str) noexcept
{
	return char2intT<uint16_t>(str.data(), str.size());
}

uint16_t str2int16(const std::string_view str) noexcept
{
	return char2intT<uint16_t>(str.data(), str.size());
}

uint32_t str2int32(const std::string& str) noexcept
{
	return char2intT<uint32_t>(str.data(), str.size());
}

uint32_t str2int32(const std::string_view str) noexcept
{
	return char2intT<uint32_t>(str.data(), str.size());
}
