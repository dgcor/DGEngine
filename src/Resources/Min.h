#pragma once

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

// a read only view of a min pillar. Pillar data is referenced, not copied.
class MinPillar : public std::span<const std::pair<uint16_t, uint16_t>>
{
public:
	MinPillar(const std::pair<uint16_t, uint16_t>* dataPtr, size_t size_)
		: std::span<const std::pair<uint16_t, uint16_t>>(dataPtr, size_) {}

	int16_t getLeftTile(size_t index) const noexcept
	{
		return (int16_t((*this)[index].first) & 0x0FFF) - 1;
	}
	int16_t getRightTile(size_t index) const noexcept
	{
		return (int16_t((*this)[index].second) & 0x0FFF) - 1;
	}

	static bool isBlank(const std::pair<uint16_t, uint16_t>& tile) noexcept
	{
		if (((int16_t(tile.first) & 0x0FFF) - 1) != -1 ||
			((int16_t(tile.second) & 0x0FFF) - 1) != -1)
		{
			return false;
		}
		return true;
	}

	// top is stored in the first elements except the last.
	bool isTopBlank() const noexcept
	{
		size_t size2 = size();
		if (size2 > 0)
		{
			size2--;
		}
		for (size_t i = 0; i < size2; i++)
		{
			if (isBlank((*this)[i]) == false)
			{
				return false;
			}
		}
		return true;
	}
};

class Min
{
private:
	std::vector<std::pair<uint16_t, uint16_t>> pillars;
	size_t numPillars{ 0 };
	size_t pillarHeight{ 0 };
	size_t numBlankTopPillars{ 0 };

public:
	Min(const std::string_view fileName, size_t minSize);

	const MinPillar operator[] (size_t index) const
	{
		return MinPillar(pillars.data() + (index * pillarHeight), pillarHeight);
	}
	auto size() const noexcept { return numPillars; }
	auto blankTopPillars() const noexcept { return numBlankTopPillars; }
};
