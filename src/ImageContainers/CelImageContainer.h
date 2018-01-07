#pragma once

#include <cstdint>
#include "ImageContainer.h"
#include <vector>

class CelImageContainer : public ImageContainer
{
private:
	std::vector<std::vector<uint8_t>> mFrames;
	size_t animLength{ 0 };
	size_t defaultWidth{ 0 };
	size_t defaultHeight{ 0 };
	bool isCl2;
	bool isTileCel;

	size_t getFrame(const std::vector<uint8_t>& frame, const PaletteArray* palette,
		size_t frameNum, std::vector<sf::Color>& rawImage) const;

	size_t readNormalFrames(sf::InputStream& file);
	size_t readCl2ArchiveFrames(sf::InputStream& file);

public:
	CelImageContainer(const char* filename, bool isCl2_, bool isTileCel_);

	// if palette is null, gets an indexed image with the index in the red channel.
	virtual sf::Image2 get(size_t index, const PaletteArray* palette) const;

	void setDefaultSize(size_t defaultWidth_, size_t defaultHeight_)
	{
		defaultWidth = defaultWidth_;
		defaultHeight = defaultHeight_;
	}

	virtual size_t size() const { return mFrames.size(); }

	///< if normal cel file, returns same as numFrames(), for an archive, the number of frames in each subcel
	size_t AnimLength() const { return animLength; }
};
