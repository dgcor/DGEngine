#pragma once

#ifndef NO_DIABLO_FORMAT_SUPPORT
#include <cstdint>
#include "ImageContainer.h"
#include <string_view>

// DC6 decoding code based on Worldstone by Lectem
// https://github.com/Lectem/Worldstone
class DC6ImageContainer : public ImageContainer
{
private:
	std::vector<uint8_t> fileData;
	uint32_t numberOfFrames{ 0 };
	uint32_t directions{ 0 };

	struct StitchedFrame
	{
		uint32_t startIndex{ 0 };
		sf::Vector2u stitch;
		sf::Vector2u size;
	};

	std::vector<StitchedFrame> stitchedFrames;
	bool useOffsets{ false };
	BlendMode blendMode{ BlendMode::Alpha };

	void calculateStitchData();

	sf::Image2 get(uint32_t startIndex, const sf::Vector2u& stitch_,
		const sf::Vector2u& size_, const PaletteArray* palette) const;

public:
	DC6ImageContainer(const std::string_view fileName, bool stitchFrames, bool useOffsets_);

	BlendMode getBlendMode() const noexcept override { return blendMode; }
	void setBlendMode(BlendMode blendMode_) noexcept override { blendMode = blendMode_; }

	sf::Image2 get(const sf::Vector2u& stitch_, const sf::Vector2u& size_,
		const PaletteArray* palette) const;

	sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const override;

	uint32_t size() const noexcept override;

	uint32_t getDirections() const noexcept override { return directions; }
};
#endif
