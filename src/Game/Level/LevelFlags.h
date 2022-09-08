#pragma once

#include <cstdint>

class LevelFlags
{
public:
	LevelFlags() = default;
	virtual ~LevelFlags() = default;

	virtual int32_t getFlags(uint32_t index, uint32_t subIndex) const { return 0; }

	int32_t getFlags(uint32_t index) const { return getFlags(index, 0); }

	static bool Passable(int32_t flags) noexcept { return !(flags & 0x01); }
};
