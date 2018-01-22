//
//	pcx.cpp - source file / freeware
//
//	David Henry - tfc_duke@hotmail.com
//

#include "Pcx.h"
#include <memory>
#include "PhysFSStream.h"

// --------------------------------------------
// PCXHEADER - pcx header structure.
// --------------------------------------------

#pragma pack(push)
#pragma pack(1)

typedef struct tagPCXHEADER
{
	unsigned char	manufacturer;		// manufacturer
	unsigned char	version;			// version
	unsigned char	encoding;			// encoding type
	unsigned char	bitsPerPixel;		// number of bits per pixel

	unsigned short	x, y;				// ...
	unsigned short	width, height;		// dimensions
	unsigned short	horzRes, vertRes;	// horisontal and vertical screen resolutions

	unsigned char*	palette;			// color palette
	unsigned char	reserved;			// reserved
	unsigned char	numColorPlanes;		// number of planes

	unsigned short	bytesPerScanLine;	// byte per line
	unsigned short	paletteType;		// palette type
	unsigned short	horzSize, vertSize;	// ...

	unsigned char	padding[54];		// ...

} PCXHEADER, *PPCXHEADER;

#pragma pack(pop)

// --------------------------------------------------
// LoadImagePCX() - load a Zsoft PCX image [.pcx]
//
// parameters :
//    - fileName [in]  : image source file
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// accepted image formats :
//     # RLE 8 bits / version 5
// -------------------------------------------------
sf::Image ImageUtils::LoadImagePCX(const char* fileName)
{
	sf::PhysFSStream stream(fileName);

	if (stream.hasError() == true ||
		stream.getSize() < 900)
	{
		return {};
	}

	auto fileSize = (size_t)stream.getSize();

	std::vector<uint8_t> buffer(fileSize + 1);
	stream.read(buffer.data(), stream.getSize());

	/////////////////////////////////////////////////////

	auto header = (PCXHEADER*)buffer.data();

	if ((header->manufacturer != 10) ||
		(header->version != 5) ||
		(header->encoding != 1) ||
		(header->bitsPerPixel != 8))
	{
		return {};
	}

	// the palette is located at the 769th last byte of the file
	auto paletteStartPos = fileSize - 769;
	auto palette = &buffer[paletteStartPos];

	// verify the palette; first byte must be equal to 12
	if (*(palette++) != 12)
	{
		return {};
	}

	auto width = header->width - header->x + 1u;
	auto height = header->height - header->y + 1u;
	auto imageSize = width * height;

	sf::Image img;
	img.create(width, height);
	auto data = (sf::Uint8*)img.getPixelsPtr();

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
	return img;
}
