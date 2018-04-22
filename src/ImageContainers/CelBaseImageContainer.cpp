#include "CelBaseImageContainer.h"

std::pair<uint16_t, uint16_t> CelBaseImageContainer::getGroupFrameIndexes(size_t groupIndex) const
{
	if (groupFrameIndexes.empty() == false &&
		groupIndex < groupCount)
	{
		return groupFrameIndexes[groupIndex];
	}
	else
	{
		return std::make_pair(0, 0);
	}
}

sf::Color CelBaseImageContainer::getColor(uint8_t palIdx, const PaletteArray* palette)
{
	return (palette == nullptr ? sf::Color(palIdx, 0, 0, 255) : (*palette)[palIdx]);
}
