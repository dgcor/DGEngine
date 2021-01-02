//
//	pcx.cpp - source file / freeware
//
//	David Henry - tfc_duke@hotmail.com
//

#include "Pcx.h"
#include <cstdint>
#include "endian/little_endian.hpp"
#include <vector>

#pragma pack(push)
#pragma pack(1)

struct PCXHeader
{
	uint8_t manufacturer;		// manufacturer
	uint8_t version;			// version
	uint8_t encoding;			// encoding type
	uint8_t bitsPerPixel;		// number of bits per pixel

	uint16_t x, y;				// ...
	uint16_t width, height;		// dimensions
	uint16_t horzRes, vertRes;	// horisontal and vertical screen resolutions

	uint8_t palette[48];		// color palette
	uint8_t reserved;			// reserved
	uint8_t numColorPlanes;		// number of planes

	uint16_t bytesPerScanLine;	// byte per line
	uint16_t paletteType;		// palette type
	uint16_t horzSize, vertSize;// ...

	uint8_t padding[54];		// ...
};

#pragma pack(pop)

bool ImageUtils::LoadImagePCX(sf::InputStream& file, sf::Image& image)
{
	if (file.getSize() < 900)
	{
		return false;
	}

	file.seek(0);

	auto fileSize = (size_t)file.getSize();

	std::vector<uint8_t> buffer(fileSize + 1);
	file.read(buffer.data(), file.getSize());

	/////////////////////////////////////////////////////

	auto header = (PCXHeader*)buffer.data();

	if ((header->manufacturer != 10) ||
		(header->version != 5) ||
		(header->encoding != 1) ||
		(header->bitsPerPixel != 8))
	{
		return false;
	}

	// the palette is located at the 769th last byte of the file
	auto paletteStartPos = fileSize - 769;

	// verify the palette; first byte must be equal to 12
	if (buffer[paletteStartPos] != 12)
	{
		return false;
	}

	// little endian handling
	header->x = endian::little_endian::get<uint16_t>((const uint8_t*)&header->x);
	header->y = endian::little_endian::get<uint16_t>((const uint8_t*)&header->y);
	header->width = endian::little_endian::get<uint16_t>((const uint8_t*)&header->width);
	header->height = endian::little_endian::get<uint16_t>((const uint8_t*)&header->height);

	auto palette = &buffer[paletteStartPos + 1];
	auto width = header->width - header->x + 1u;
	auto height = header->height - header->y + 1u;
	auto imageSize = width * height;

	image.create(width, height);
	auto data = (sf::Uint8*)image.getPixelsPtr();

	// decode compressed image (RLE)
	for (size_t idx = 0, buffIdx = 128;
		idx < imageSize && buffIdx < paletteStartPos;)
	{
		auto c = buffer[buffIdx++];
		if (c > 0xbf)
		{
			size_t numRepeat = 0x3f & c;
			c = buffer[buffIdx++];

			for (size_t i = 0; i < numRepeat; i++)
			{
				auto palIdx = 3u * c;
				data[idx * 4 + 0] = palette[palIdx + 0];
				data[idx * 4 + 1] = palette[palIdx + 1];
				data[idx * 4 + 2] = palette[palIdx + 2];
				idx++;
				if (idx >= imageSize)
				{
					break;
				}
			}
		}
		else
		{
			auto palIdx = 3u * c;
			data[idx * 4 + 0] = palette[palIdx + 0];
			data[idx * 4 + 1] = palette[palIdx + 1];
			data[idx * 4 + 2] = palette[palIdx + 2];
			idx++;
		}
	}
	return true;
}
