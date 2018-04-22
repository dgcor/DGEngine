#pragma once

#include <cstdint>
#include "ImageContainer.h"

class CelBaseImageContainer : public ImageContainer
{
protected:
	enum class CelType
	{
		None,
		V1Regular,
		V1Compilation,
		V1Level,
		V2MonoGroup,
		V2MultipleGroups
	};

	CelType type{ CelType::None };
	std::vector<uint8_t> fileData;
	uint16_t groupCount{ 0 };
	std::vector<std::pair<uint16_t, uint16_t>> groupFrameIndexes;
	std::vector<std::pair<uint32_t, uint32_t>> frameOffsets;

public:
	static sf::Color getColor(uint8_t palIdx, const PaletteArray* palette);

	virtual size_t size() const noexcept { return frameOffsets.size(); }
	uint16_t getGroupCount() const noexcept { return groupCount; }
	std::pair<uint16_t, uint16_t> getGroupFrameIndexes(size_t groupIndex) const;
};
