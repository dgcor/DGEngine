#include "CelUtils.h"
#include "CelCache.h"
#include "FileUtils.h"
#include "Utils.h"

sf::Image CelUtils::loadImage(const char* fileName, const Palette& pal, bool isCl2)
{
	CelFile celFile(fileName, isCl2, false);
	CelFrameCache cel(celFile, pal);

	size_t imgWidth = 0;
	size_t imgHeight = 0;
	for (size_t i = 0; i < cel.size(); i++)
	{
		const auto& frame = cel[i];
		if (imgWidth < frame.Width())
		{
			imgWidth = frame.Width();
		}
		imgHeight += frame.Height();
	}

	sf::Image img;
	img.create(imgWidth, imgHeight, sf::Color::Transparent);

	size_t maxHeight = 0;
	for (size_t fr = 0; fr < cel.size(); fr++)
	{
		const auto& frame = cel[fr];
		for (size_t j = 0; j < frame.Height(); j++)
		{
			for (size_t i = 0; i < frame.Width(); i++)
			{
				const auto& color = frame[i][j];
				img.setPixel(i, maxHeight + j, color);
			}
		}
		maxHeight += frame.Height();
	}
	return img;
}

sf::Image CelUtils::loadImage(const char* fileName, const Palette& pal, bool isCl2,
	size_t& frameCountX, size_t& frameCountY)
{
	CelFile celFile(fileName, isCl2, false);
	CelFrameCache cel(celFile, pal);

	auto numFramesY = cel.size();

	if (numFramesY == 0)
	{
		return sf::Image();
	}

	numFramesY /= (frameCountX);

	auto numFramesX = (frameCountX);

	sf::Image img;
	img.create(cel[0].Width() * numFramesX, cel[0].Height() * numFramesY, sf::Color::Transparent);

	size_t maxWidth = 0;
	size_t maxHeight = 0;
	for (size_t frX = 0; frX < numFramesX; frX++)
	{
		for (size_t frY = 0; frY < numFramesY; frY++)
		{
			const auto& frame = cel[(frX * numFramesY) + frY];

			for (size_t j = 0; j < frame.Height(); j++)
			{
				for (size_t i = 0; i < frame.Width(); i++)
				{
					const auto& color = frame[i][j];
					img.setPixel(maxWidth + i, maxHeight + j, color);
				}
			}

			maxHeight += frame.Height();
		}

		maxWidth += cel[0].Width();
		maxHeight = 0;
	}

	frameCountY = numFramesY;

	return img;
}

sf::Image CelUtils::loadImageFrame(const char* fileName, const Palette& pal, bool isCl2,
	size_t frameIdx)
{
	CelFile celFile(fileName, isCl2, false);

	if (celFile.Size() > 0 && frameIdx < celFile.Size())
	{
		return celFile.get(frameIdx, pal);
	}

	return sf::Image();
}

sf::Image CelUtils::loadBitmapFontImage(const char* fileName, const char* fileNameBin,
	const Palette& pal, bool isCl2)
{
	CelFile celFile(fileName, isCl2, false);
	CelFrameCache cel(celFile, pal);

	auto celSize = cel.size();
	if (celSize == 0)
	{
		return sf::Image();
	}

	sf::Image img;
	img.create(cel[0].Width() * 16, cel[0].Height() * 16, sf::Color::Transparent);
	auto charMapping = FileUtils::readChar(fileNameBin, 256);

	size_t xx = 0;
	size_t yy = 0;
	for (auto charMap : charMapping)
	{
		if (charMap != 0xFF && charMap < celSize)
		{
			const auto& frame = cel[charMap];

			for (size_t j = 0; j < frame.Height(); j++)
			{
				for (size_t i = 0; i < frame.Width(); i++)
				{
					const auto& color = frame[i][j];
					img.setPixel(frame.Width() * xx + i, frame.Height() * yy + j, color);
				}
			}
		}
		xx++;
		if (xx == 16)
		{
			xx = 0;
			yy++;
		}
	}
	return img;
}
