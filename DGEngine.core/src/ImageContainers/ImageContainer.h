#pragma once

#include "BlendMode.h"
#include <cstdint>
#include "Palette.h"
#include "SFML/Image2.h"

class ImageContainer
{
public:
	struct ImageInfo
	{
		sf::Vector2f offset;
		bool absoluteOffset{ false };
		BlendMode blendMode{ BlendMode::Alpha };
		int32_t nextIndex{ -1 };
	};

	virtual ~ImageContainer() = default;

	virtual BlendMode getBlendMode() const noexcept = 0;
	virtual void setBlendMode(BlendMode blendMode_) noexcept = 0;

	virtual sf::Image2 get(uint32_t index, const PaletteArray* palette, ImageInfo& imgInfo) const = 0;

	sf::Image2 get(uint32_t index, const PaletteArray* palette) const
	{
		ImageInfo imgInfo;
		return get(index, palette, imgInfo);
	}

	virtual uint32_t size() const noexcept = 0;

	virtual uint32_t getDirections() const noexcept = 0;

	static sf::Color getColor(uint8_t palIdx, const PaletteArray* palette)
	{
		return (palette == nullptr ? sf::Color(palIdx, 0, 0, 255) : (*palette)[palIdx]);
	}
};
