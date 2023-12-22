#include "Palette.h"
#include <algorithm>
#include "Utils/Log.h"
#include "Utils/NumberVector.h"

Palette::Palette(const std::string_view file, ColorFormat colorFormat)
{
	NumberVector<uint8_t> bytes(file, 0, 1024);

	if (bytes.size() < 768)
	{
		SPDLOG_WARN("Invalid palette. using grayscale palette.");
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
		bool has4ByteColor = bytes.size() == 1024;
		size_t i = 0;
		for (auto& color : palette)
		{
			color.r = bytes[i++];
			color.g = bytes[i++];
			color.b = bytes[i++];
			if (colorFormat >= ColorFormat::RGBA && has4ByteColor == true)
			{
				color.a = bytes[i++];
			}
			switch (colorFormat)
			{
			case ColorFormat::BGR:
			case ColorFormat::BGRA:
				std::swap(color.r, color.b);
				break;
			case ColorFormat::ARGB:
			{
				if (has4ByteColor == true)
				{
					std::swap(color.b, color.a);
					std::swap(color.a, color.r);
					std::swap(color.r, color.g);
				}
				break;
			}
			case ColorFormat::ABGR:
			{
				if (has4ByteColor == true)
				{
					std::swap(color.a, color.r);
					std::swap(color.b, color.g);
				}
				break;
			}
			default:
				break;
			}
		}
	}
	loadTexture();
}

Palette::Palette(const Palette& pal, const FileBytes& trn, size_t start, size_t length)
{
	if (start + length <= trn.size())
	{
		for (size_t i = 0; i < length; i++)
		{
			palette[i] = pal[(size_t)trn[start + i]];
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

bool Palette::replaceRange(const Palette& srcPal, size_t srcStartIdx, size_t size, size_t dstStartIdx)
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
