#pragma once

#include <vector>
#include <cstdint>

#include "Palette.h"
#include "Helper2D.h"

class CelFrame
{
private:
	std::vector<sf::Color> rawImage;
	size_t width;
	size_t height;

public:
	CelFrame() {}
	CelFrame(const std::vector<sf::Color>& rawImage_, size_t width_, size_t height_) :
		rawImage(rawImage_), width(width_), height(height_) {}

	size_t Width() const { return width; }
	size_t Height() const { return height; }
	const std::vector<sf::Color>& RawImage() const { return rawImage; }

	static const sf::Color& get(size_t x, size_t y, const CelFrame& frame)
	{
		return frame.rawImage[x + (frame.height - 1 - y) * frame.width];
	}

	Misc::Helper2D<const CelFrame, const sf::Color&, size_t> operator[] (size_t x) const
	{
		return Misc::Helper2D<const CelFrame, const sf::Color&, size_t>(*this, x, get);
	}

	operator sf::Image() const;
	operator sf::Texture() const;
};

class CelFile
{
private:
	std::vector<std::vector<uint8_t>> mFrames;
	size_t animLength{ 0 };
	bool isCl2;
	bool isTileCel;

	size_t getFrame(const std::vector<uint8_t>& frame, const Palette& palette,
		size_t frameNum, std::vector<sf::Color>& rawImage) const;

	size_t readNormalFrames(sf::InputStream& file);
	size_t readCl2ArchiveFrames(sf::InputStream& file);

public:
	CelFile(const char* filename, bool isCl2_, bool isTileCel_);

	CelFrame get(size_t index, const Palette& palette) const;

	size_t Size() const { return mFrames.size(); }

	///< if normal cel file, returns same as numFrames(), for an archive, the number of frames in each subcel
	size_t AnimLength() const { return animLength; }
};
