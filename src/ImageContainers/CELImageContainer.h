#pragma once

#include <cstdint>
#include "FileBytes.h"
#include "ImageContainers/ImageContainer.h"
#include <memory>

// CEL decoding code based on Diablo 1 Graphics Tool by savagesteel
// https://github.com/savagesteel/d1-graphics-tool
class CELImageContainer : public ImageContainer
{
private:
	enum class CelType
	{
		None,
		V1Regular,
		V1Compilation,
		V1Level
	};

	CelType type{ CelType::None };
	std::shared_ptr<FileBytes> fileData;
	std::vector<std::pair<uint32_t, uint32_t>> frameOffsets;
	uint32_t directions{ 0 };
	BlendMode blendMode{ BlendMode::Alpha };

public:
	CELImageContainer(const std::shared_ptr<FileBytes>& fileBytes);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray * palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override { return (uint32_t)frameOffsets.size(); }

	uint32_t getDirections() const noexcept override { return directions; }
};
