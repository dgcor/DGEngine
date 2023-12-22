#include "LazyImageContainer.h"

sf::Image2 LazyImageContainer::get(uint32_t index,
	const PaletteArray* palette, ImageInfo& imgInfo) const
{
	if (index >= frameOffsets.size())
	{
		return {};
	}

	imgInfo.offset = {};
	imgInfo.absoluteOffset = false;
	imgInfo.blendMode = blendMode;
	imgInfo.nextIndex = -1;

	uint32_t frameSize = frameOffsets[index].second - frameOffsets[index].first;
	std::span<const uint8_t> frameData(&(uint8_t&)(*fileData)[frameOffsets[index].first], frameSize);

	return decode(frameData, palette);
}
