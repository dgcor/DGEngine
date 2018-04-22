#pragma once

#include <cstdint>
#include <string>
#include <vector>

class MinPillar : public std::vector<std::pair<uint16_t, uint16_t>>
{
public:
	int16_t getLeftTile(size_t index) const noexcept
	{
		return (int16_t((*this)[index].first) & 0x0FFF) - 1;
	}
	int16_t getRightTile(size_t index) const noexcept
	{
		return (int16_t((*this)[index].second) & 0x0FFF) - 1;
	}
};

class Min
{
private:
	std::vector<MinPillar> pillars;

public:
	Min(const std::string& fileName, size_t minSize);

	const MinPillar& operator[] (size_t index) const
	{
		return pillars[index];
	}
	size_t size() const noexcept { return pillars.size(); }
};
