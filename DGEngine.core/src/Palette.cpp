#include "Palette.h"
#include <algorithm>
#include "SFML/PhysFSStream.h"

Palette::Palette(const std::string_view file, ColorFormat colorFormat)
{
	sf::PhysFSStream stream(file.data());

	if (stream.hasError() == true || stream.getSize() < 768)
	{
		uint8_t c = 0;
		for (auto& color : palette)
		{
			color.r = c;
			color.g = c;
			color.b = c;
			c++;
		}
	}
	else
	{
		for (auto& color : palette)
		{
			stream.read(&color.r, 1);
			stream.read(&color.g, 1);
			stream.read(&color.b, 1);
			if (colorFormat >= ColorFormat::RGBA)
			{
				stream.read(&color.a, 1);
			}
			switch (colorFormat)
			{
			case ColorFormat::BGR:
			case ColorFormat::BGRA:
				std::swap(color.r, color.b);
				break;
			case ColorFormat::ARGB:
				color = sf::Color(color.toInteger() << 8);
				break;
			case ColorFormat::ABGR:
			{
				color = sf::Color(color.toInteger() << 8);
				std::swap(color.r, color.b);
				break;
			}
			default:
				break;
			}
		}
	}
	loadTexture();
}

Palette::Palette(const Palette& pal, const std::vector<sf::Uint8> trn,
	size_t start, size_t length)
{
	if (start + length <= trn.size())
	{
		for (size_t i = 0; i < length; i++)
		{
			palette[i] = pal[trn[start + i]];
		}
	}
	loadTexture();
}

void Palette::loadTexture()
{
	sf::Image img;
	img.create((unsigned)palette.size(), 1, (const sf::Uint8*)&palette);
	texture.loadFromImage(img);
}

void Palette::updateTexture()
{
	texture.update((const sf::Uint8*)&palette, (unsigned)palette.size(), 1, 0, 0);
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
