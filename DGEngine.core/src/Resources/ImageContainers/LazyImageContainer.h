#pragma once

#include <cstdint>
#include <memory>
#include "Resources/FileBytes.h"
#include "Resources/ImageContainer.h"
#include <span>

// Lazy loading image container for common formats
class LazyImageContainer : public ImageContainer
{
protected:
	std::shared_ptr<FileBytes> fileData;
	std::vector<std::pair<uint32_t, uint32_t>> frameOffsets;
	BlendMode blendMode{ BlendMode::Alpha };

	virtual sf::Image2 decode(const std::span<const uint8_t> frameData, const PaletteArray* palette) const = 0;

public:
	LazyImageContainer(const std::shared_ptr<FileBytes>& fileData_) : fileData(fileData_) {}

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override { return (uint32_t)frameOffsets.size(); }

	uint32_t getDirections() const noexcept override { return 1; }
};
