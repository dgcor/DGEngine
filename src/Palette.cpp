#include "Palette.h"
#include <algorithm>
#include "PhysFSStream.h"

Palette::Palette(const std::string_view file)
{
	sf::PhysFSStream stream(file.data());

	if (stream.hasError() == true || stream.getSize() < 768)
	{
		throw std::exception();
	}

	for (auto& color : palette)
	{
		stream.read(&color, 3);
	}
	loadTexture();
}

Palette::Palette(const Palette& pal, const std::vector<sf::Uint8> trn)
{
	for (size_t i = 0; i < palette.size(); i++)
	{
		palette[i] = pal[trn[i]];
	}
	loadTexture();
}

void Palette::loadTexture()
{
	sf::Image img;
	img.create(palette.size(), 1, (const sf::Uint8*)&palette);
	texture.loadFromImage(img);
}

void Palette::updateTexture()
{
	texture.update((const sf::Uint8*)&palette, palette.size(), 1, 0, 0);
}

bool Palette::shiftLeft(size_t shift, size_t startIdx, size_t stopIdx)
{
	if (stopIdx <= palette.size() &&
		startIdx < stopIdx)
	{
		auto range = stopIdx - startIdx;
		std::rotate(palette.begin() + startIdx,
			palette.begin() + startIdx + (shift % range),
			palette.begin() + stopIdx);
		updateTexture();
		return true;
	}
	return false;
}

bool Palette::shiftRight(size_t shift, size_t startIdx, size_t stopIdx)
{
	if (stopIdx <= palette.size() &&
		startIdx < stopIdx)
	{
		auto range = stopIdx - startIdx;
		std::rotate(palette.rbegin() + (palette.size() - stopIdx),
			palette.rbegin() + (palette.size() - stopIdx + (shift % range)),
			palette.rbegin() + (palette.size() - startIdx));
		updateTexture();
		return true;
	}
	return false;
}

bool Palette::replaceRange(const Palette& srcPal, size_t srcStartIdx,
	size_t size, size_t dstStartIdx)
{
	if (srcStartIdx + size <= srcPal.palette.size() &&
		dstStartIdx + size <= palette.size())
	{
		std::copy_n(srcPal.palette.begin() + srcStartIdx,
			size,
			palette.begin() + dstStartIdx);
		updateTexture();
		return true;
	}
	return false;
}
