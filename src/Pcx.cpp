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

	if (stream.hasError() == true)
	{
		return sf::Image();
	}

	long flen = (long)stream.getSize();

	auto buffer = std::make_unique<char[]>(flen + 1);
	stream.read(buffer.get(), flen);
	char* pBuff = buffer.get();

	/////////////////////////////////////////////////////

	auto header = (PCXHEADER*)pBuff;

	if ((header->manufacturer != 10) ||
		(header->version != 5) ||
		(header->encoding != 1) ||
		(header->bitsPerPixel != 8))
	{
		return sf::Image();
	}

	header->width = header->width - header->x + 1;
	header->height = header->height - header->y + 1;

	// allocate memory for image data
	auto data = std::make_unique<unsigned char[]>(header->width * header->height);

	pBuff = (char*)&buffer[128];

	unsigned char c;

	// uncode compressed image (RLE)
	for (int idx = 0; idx < (header->width * header->height);)
	{
		if ((c = *(pBuff++)) > 0xbf)
		{
			auto numRepeat = 0x3f & c;
			c = *(pBuff++);

			for (int i = 0; i < numRepeat; i++) {
				data[idx++] = c;
			}
		}
		else {
			data[idx++] = c;
		}
	}

	// the palette is located at the 769th last byte of the file
	pBuff = &buffer[flen - 769];

	// verify the palette; first char must be equal to 12
	if (*(pBuff++) != 12)
	{
		return sf::Image();
	}

	// read the palette
	header->palette = (unsigned char*)pBuff;

	sf::Image img;
	img.create(header->width, header->height);

	for (auto i = 0; i < header->width; i++)
	{
		for (auto j = 0; j < header->height; j++)
		{
			auto color = 3 * data[i + (j * header->width)];
			auto rgba = sf::Color(
				header->palette[color + 0],
				header->palette[color + 1],
				header->palette[color + 2]);

			img.setPixel(i, j, rgba);
		}
	}

	return img;
}
