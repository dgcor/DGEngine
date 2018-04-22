#pragma once

#include "CelBaseImageContainer.h"
#include "gsl/gsl"

class CelImageContainer : public CelBaseImageContainer
{
private:
	// Class used only for CEL frame width calculation
	class CelPixelGroup
	{
	private:
		uint16_t pixelCount;
		bool transparent;

	public:
		CelPixelGroup(uint16_t pixelCount_, bool transparent_) :
			pixelCount(pixelCount_), transparent(transparent_) {}

		uint16_t getPixelCount() const noexcept { return pixelCount; }
		bool isTransparent() const noexcept { return transparent; }
	};

	enum class CelFrameType
	{
		Regular,	// == LEVEL_TYPE_1
		LevelType0,	// 0x400 full opaque
		LevelType2,	// 0x220 left transparency
		LevelType3,	// 0x220 right transparency
		LevelType4,	// 0x320 left transparency
		LevelType5	// 0x320 right transparency
	};

	static CelFrameType getLevelFrame220Type(const gsl::span<const uint8_t> frameData);
	static CelFrameType getLevelFrame320Type(const gsl::span<const uint8_t> frameData);
	static CelFrameType getLevelFrame400Type(const gsl::span<const uint8_t> frameData);
	static uint16_t computeWidthFromHeader(const gsl::span<const uint8_t> frameData);
	static uint16_t computeWidthFromData(const gsl::span<const uint8_t> frameData);

	static sf::Image2 decode(const gsl::span<const uint8_t> frameData,
		unsigned width, unsigned height, CelFrameType frameType,
		const PaletteArray* palette);

public:
	CelImageContainer(const char* fileName);

	virtual sf::Image2 get(size_t index, const PaletteArray * palette) const;
};
