#pragma once

#include <cstdint>
#include "Resources/FileBytes.h"
#include "Resources/ImageContainer.h"
#include <memory>

// DCC decoding code based on Worldstone by Lectem
// https://github.com/Lectem/Worldstone
class DCCImageContainer : public ImageContainer
{
private:
	std::shared_ptr<FileBytes> fileData;
	uint32_t numberOfFrames{ 0 };
	uint32_t directions{ 0 };
	uint32_t framesPerDir{ 0 };
	BlendMode blendMode{ BlendMode::Alpha };

	// Offset of each direction header in the file
	// holds directions + 1 values, the last one being the size of the file
	std::vector<uint32_t> directionsOffsets;
	std::vector<uint32_t> framePointers;

public:
	DCCImageContainer(const std::shared_ptr<FileBytes>& fileBytes);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override { return numberOfFrames; }

	uint32_t getDirections() const noexcept override { return directions; }
};
