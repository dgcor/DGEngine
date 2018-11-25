#include "CelImageContainer.h"
#include "PhysFSStream.h"
#include "StreamReader.h"

const bool CelLevelFrameType2[512] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const uint16_t CelLevelFrameType2ZeroedBytes[32] =
{
	0, 1, 8, 9, 24, 25, 48, 49, 80, 81, 120, 121, 168, 169, 224, 225,
	288, 289, 348, 349, 400, 401, 444, 445, 480, 481, 508, 509, 528, 529, 540, 541
};

const bool CelLevelFrameType3[512] =
{
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const uint16_t CelLevelFrameType3ZeroedBytes[32] =
{
	2, 3, 14, 15, 34, 35, 62, 63, 98, 99, 142, 143, 194, 195, 254, 255,
	318, 319, 374, 375, 422, 423, 462, 463, 494, 495, 518, 519, 534, 535, 542, 543
};

const bool CelLevelFrameType4[512] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

const uint16_t CelLevelFrameType4ZeroedBytes[16] =
{
	0, 1, 8, 9, 24, 25, 48, 49, 80, 81, 120, 121, 168, 169, 224, 225
};

const bool CelLevelFrameType5[512] =
{
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

const uint16_t CelLevelFrameType5ZeroedBytes[16] =
{
	2, 3, 14, 15, 34, 35, 62, 63, 98, 99, 142, 143, 194, 195, 254, 255
};

CelImageContainer::CelFrameType CelImageContainer::getLevelFrame220Type(
	const gsl::span<const uint8_t> frameData)
{
	auto type = CelFrameType::Regular;
	for (size_t i = 0; i < 32; i++)
	{
		if (frameData[CelLevelFrameType2ZeroedBytes[i]] != 0)
		{
			break;
		}
		if (i == 31)
		{
			type = CelFrameType::LevelType2;
		}
	}
	for (size_t i = 0; i < 32; i++)
	{
		if (frameData[CelLevelFrameType3ZeroedBytes[i]] != 0)
		{
			break;
		}
		if (i == 31)
		{
			type = CelFrameType::LevelType3;
		}
	}
	return type;
}

CelImageContainer::CelFrameType CelImageContainer::getLevelFrame320Type(
	const gsl::span<const uint8_t> frameData)
{
	auto type = CelFrameType::Regular;
	for (size_t i = 0; i < 16; i++)
	{
		if (frameData[CelLevelFrameType4ZeroedBytes[i]] != 0)
		{
			break;
		}
		if (i == 15)
		{
			type = CelFrameType::LevelType4;
		}
	}
	for (size_t i = 0; i < 16; i++)
	{
		if (frameData[CelLevelFrameType5ZeroedBytes[i]] != 0)
		{
			break;
		}
		if (i == 15)
		{
			type = CelFrameType::LevelType5;
		}
	}
	return type;
}

CelImageContainer::CelFrameType CelImageContainer::getLevelFrame400Type(
	const gsl::span<const uint8_t> frameData)
{
	uint32_t globalPixelCount = 0;
	uint16_t pixelCount = 0;

	// Going through the frame data to find pixel groups
	for (ptrdiff_t i = 0; i < frameData.size(); i++)
	{
		auto readByte = frameData[i];

		// Transparent pixels group
		if (readByte > 0x80)
		{
			pixelCount += (256u - readByte);
			globalPixelCount += pixelCount;
			if (pixelCount > 32)
			{
				return CelFrameType::LevelType0;
			}
			pixelCount = 0;
		}
		else if (readByte == 0x80)
		{
			pixelCount += 0x80;
			if (pixelCount > 32)
			{
				return CelFrameType::LevelType0;
			}
		}
		// Palette indices pixel group
		else if (readByte == 0x7F)
		{
			pixelCount += 0x7F;
			if (pixelCount > 32 || i + 0x7F >= frameData.size())
			{
				return CelFrameType::LevelType0;
			}
			i += 0x7F;
		}
		else
		{
			pixelCount += readByte;
			globalPixelCount += pixelCount;
			if (pixelCount > 32 || i + readByte >= frameData.size())
			{
				return CelFrameType::LevelType0;
			}
			pixelCount = 0;
			i += readByte;
		}
	}
	if (globalPixelCount == 32 * 32)
	{
		return CelFrameType::Regular;
	}
	return CelFrameType::LevelType0;
}

uint16_t CelImageContainer::computeWidthFromHeader(const gsl::span<const uint8_t> frameData)
{
	std::array<uint16_t, 5> celFrameHeader;
	std::array<uint16_t, 4> celFrameWidth = { 0, 0, 0, 0 };

	// Read the {CEL FRAME HEADER}
	LittleEndianStreamReader fileStream(frameData.data(), frameData.size());
	for (auto& headerElem : celFrameHeader)
	{
		fileStream.read(headerElem);
	}

	// Read the five 32 pixel-lines block to calculate the image width
	uint16_t pixelCount = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (celFrameHeader[i + 1] == 0)
		{
			break;
		}
		for (size_t j = celFrameHeader[i]; j < celFrameHeader[i + 1]; j++)
		{
			auto readByte = frameData[(ptrdiff_t)j];

			if (readByte > 0x7F)
			{
				pixelCount += (256u - readByte);
			}
			else
			{
				pixelCount += readByte;
				j += readByte;
			}
		}

		celFrameWidth[i] = pixelCount / 32;
		pixelCount = 0;
	}

	// The calculated width has to be the identical for each 32 pixel-line block
	// If it's not the case, 0 is returned
	for (size_t i = 0; i < 3; i++)
	{
		if (celFrameWidth[i + 1] != 0 && celFrameWidth[i] != celFrameWidth[i + 1])
		{
			return 0;
		}
	}
	return celFrameWidth[0];
}

uint16_t CelImageContainer::computeWidthFromData(const gsl::span<const uint8_t> frameData)
{
	int32_t frameDataStartOffset = 0;
	uint32_t globalPixelCount = 0;
	uint16_t biggestGroupPixelCount = 0;
	uint16_t pixelCount = 0;
	std::vector<CelPixelGroup> pixelGroups;

	// Checking the presence of the {CEL FRAME HEADER}
	if (frameData[0] == 0x0A && frameData[1] == 0x00)
	{
		frameDataStartOffset = 0x0A;
	}

	// Going through the frame data to find pixel groups
	for (ptrdiff_t i = frameDataStartOffset; i < frameData.size(); i++)
	{
		auto readByte = frameData[i];

		// Transparent pixels group
		if (readByte > 0x80)
		{
			pixelCount += (256u - readByte);
			pixelGroups.push_back(CelPixelGroup(pixelCount, true));
			globalPixelCount += pixelCount;
			if (pixelCount > biggestGroupPixelCount)
			{
				biggestGroupPixelCount = pixelCount;
			}
			pixelCount = 0;
		}
		else if (readByte == 0x80)
		{
			pixelCount += 0x80;
		}
		// Palette indices pixel group
		else if (readByte == 0x7F)
		{
			pixelCount += 0x7F;
			i += 0x7F;
		}
		else
		{
			pixelCount += readByte;
			pixelGroups.push_back(CelPixelGroup(pixelCount, false));
			globalPixelCount += pixelCount;
			if (pixelCount > biggestGroupPixelCount)
			{
				biggestGroupPixelCount = pixelCount;
			}
			pixelCount = 0;
			i += readByte;
		}
	}

	// Going through pixel groups to find pixel-lines wraps
	uint16_t width = 0;
	pixelCount = 0;
	for (size_t i = 1; i < pixelGroups.size(); i++)
	{
		pixelCount += pixelGroups[i - 1].getPixelCount();

		if (pixelGroups[i - 1].isTransparent() == pixelGroups[i].isTransparent())
		{
			// If width == 0 then it's the first pixel-line wrap and width needs to be set
			// If pixelCount is less than width then the width has to be set to the new value
			if (width == 0 || pixelCount < width)
			{
				width = pixelCount;
			}
			// If the pixelCount of the last group is less than the current pixel group
			// then width is equal to this last pixel group's pixel count.
			// Mostly useful for small frames like the "J" frame in smaltext.cel
			if (i == pixelGroups.size() - 1 && pixelGroups[i].getPixelCount() < pixelCount)
			{
				width = pixelGroups[i].getPixelCount();
			}
			pixelCount = 0;
		}
		// If last pixel group is being processed and width is still unknown
		// then set the width to the pixelCount of the last two pixel groups
		if (i == pixelGroups.size() - 1 && width == 0)
		{
			width = pixelGroups[i - 1].getPixelCount() + pixelGroups[i].getPixelCount();
		}
	}
	// If width wasn't found return 0
	if (width == 0)
	{
		return 0;
	}
	// If width is consistent
	if (globalPixelCount % width == 0)
	{
		return width;
	}
	// If width is inconsistent
	else
	{
		// Try to find  relevant width by adding pixel groups' pixel counts iteratively
		pixelCount = 0;
		for (const auto& pixelGroup : pixelGroups)
		{
			pixelCount += pixelGroup.getPixelCount();
			if (pixelCount > 1 &&
				globalPixelCount % pixelCount == 0 &&
				pixelCount >= biggestGroupPixelCount)
			{
				return pixelCount;
			}
		}
		// If still no width found return 0
		return 0;
	}
}

sf::Image2 CelImageContainer::decode(const gsl::span<const uint8_t> frameData,
	unsigned width, unsigned height, CelFrameType frameType, const PaletteArray* palette)
{
	uint32_t frameDataStartOffset = 0;
	sf::Image2 img;

	// If the frame size wasnt provided then it needs to be calculated
	if (width == 0)
	{
		// Checking the presence of the {CEL FRAME HEADER}
		if (frameData[0] == 0x0A && frameData[1] == 0x00)
		{
			frameDataStartOffset += 0x0A;
			// If header is present, try to compute frame width from frame header
			width = computeWidthFromHeader(frameData);
		}
		// If width could not be calculated with frame header,
		// attempt to calculate it from the frame data (by identifying pixel groups line wraps)
		if (width == 0)
		{
			width = computeWidthFromData(frameData);
		}
		// if CEL width was not found, return false
		if (width == 0)
		{
			return img;
		}
	}

	// READ {CEL FRAME DATA}
	// if it is a CEL level frame
	if (frameType != CelFrameType::Regular)
	{
		img.create(width, height, sf::Color::Transparent);

		// 0x400 frame
		if (frameType == CelFrameType::LevelType0)
		{
			for (int j = 0; j < 32; j++)
			{
				for (int i = 0; i < 32; i++)
				{
					img.setPixel(i, 31 - j,
						CelBaseImageContainer::getColor(frameData[j * 32 + i], palette));
				}
			}
		}
		// 0x220 or 0x320 frame
		else
		{
			const bool* dataPattern = nullptr;
			const uint16_t* dataPatternZeroedBytes = nullptr;

			switch (frameType)
			{
			case CelFrameType::LevelType2:
				dataPattern = CelLevelFrameType2;
				dataPatternZeroedBytes = CelLevelFrameType2ZeroedBytes;
				break;
			case CelFrameType::LevelType3:
				dataPattern = CelLevelFrameType3;
				dataPatternZeroedBytes = CelLevelFrameType3ZeroedBytes;
				break;
			case CelFrameType::LevelType4:
				dataPattern = CelLevelFrameType4;
				dataPatternZeroedBytes = CelLevelFrameType4ZeroedBytes;
				break;
			case CelFrameType::LevelType5:
				dataPattern = CelLevelFrameType5;
				dataPatternZeroedBytes = CelLevelFrameType5ZeroedBytes;
				break;
			default:
				return img;
			}

			// Going through the data structure
			int offset = 0;
			int zeroedBytesIndex = 0;
			unsigned currWidth = 0;
			unsigned currHeight = 31;
			for (size_t i = 0; i < 512; i++)
			{
				// if dataPattern[i] is true, then read and add 2 pixels to the line
				if (dataPattern[i])
				{
					auto readByte = frameData[offset];
					auto secondReadByte = frameData[offset + 1];

					if (readByte == 0x00 && secondReadByte == 0x00
						&& offset == dataPatternZeroedBytes[zeroedBytesIndex])
					{
						// Skip the 0x00 0x00 bytes
						offset += 2;
						// and read the next 2 bytes
						readByte = frameData[offset];
						secondReadByte = frameData[offset + 1];

						// move forward in the zeroed bytes structure
						zeroedBytesIndex += 2;
					}

					img.setPixel(currWidth++, currHeight,
						CelBaseImageContainer::getColor(readByte, palette));
					img.setPixel(currWidth++, currHeight,
						CelBaseImageContainer::getColor(secondReadByte, palette));

					offset += 2;
				}
				// else add 2 transparent pixels to the line
				else
				{
					//img.setPixel(currWidth++, currHeight, sf::Color::Transparent);
					//img.setPixel(currWidth++, currHeight, sf::Color::Transparent);
					currWidth += 2;
				}
				// If it is the end of the pixel line, add the line to the frame
				if (currWidth == 32)
				{
					currHeight--;
					currWidth = 0;
				}
			}
		}
		return img;
	}
	// if it's a regular CEL frame
	else
	{
		std::vector<sf::Color> pixels;
		pixels.reserve(width * width);
		unsigned currWidth = 0;
		for (ptrdiff_t i = frameDataStartOffset; i < frameData.size(); i++)
		{
			auto readByte = frameData[i];

			// Transparent pixels group
			if (readByte > 0x7F)
			{
				// A pixel line can't exceed the image width
				if ((currWidth + (256u - readByte)) > width)
				{
					return img;
				}
				auto rangeEnd = (256u - readByte);
				currWidth += rangeEnd;
				for (size_t j = 0; j < rangeEnd; j++)
				{
					pixels.push_back(sf::Color::Transparent);
				}
			}
			// Palette indices group
			else
			{
				// A pixel line can't exceed the image width
				if ((currWidth + readByte) > width)
				{
					return img;
				}
				currWidth += readByte;
				for (size_t j = 0; j < readByte; j++)
				{
					i++;
					pixels.push_back(CelBaseImageContainer::getColor(frameData[i], palette));
				}
			}
			if (currWidth == width)
			{
				currWidth = 0;
			}
		}
		if (height == 0)
		{
			height = pixels.size() / width;
		}

		img.create(width, height, (const sf::Uint8*)pixels.data());
		img.flipVertically();
		return img;
	}
}

CelImageContainer::CelImageContainer(const std::string_view fileName)
{
	type = CelType::V1Regular;

	uint32_t firstDword = 0;
	uint32_t fileSizeDword = 0;
	uint32_t lastCelOffset = 0;
	uint32_t lastCelFrameCount = 0;
	uint32_t lastCelSize = 0;

	uint32_t celOffset = 0;
	uint32_t celFrameCount = 0;
	uint32_t celFrameStartOffset = 0;
	uint32_t celFrameEndOffset = 0;
	uint32_t celFrameSize = 0;

	{
		sf::PhysFSStream file(fileName.data());
		// Opening CL2 file with a QBuffer to load it in RAM
		if (file.hasError() == true)
		{
			return;
		}
		fileData.resize((size_t)file.getSize());
		file.read(fileData.data(), file.getSize());
	}

	LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

	// CEL HEADER CHECKS

	// Read first DWORD
	fileStream.read(firstDword);

	// Trying to find file size in CEL header
	if (fileData.size() < ((uint64_t)firstDword * 4 + 4 + 4))
	{
		return;
	}

	fileStream.seek((uint64_t)firstDword * 4 + 4);
	fileStream.read(fileSizeDword);

	// If the dword is not equal to the file size then
	// check if it's a CEL compilation
	if (fileData.size() != fileSizeDword)
	{
		// Read offset of the last CEL of the CEL compilation
		fileStream.seek(firstDword - 4);
		fileStream.read(lastCelOffset);

		// Go to last CEL of the CEL compilation
		if (fileData.size() < (lastCelOffset + 8))
		{
			return;
		}

		fileStream.seek(lastCelOffset);

		// Read last CEL header
		fileStream.read(lastCelFrameCount);

		// Read the last CEL size
		if (fileData.size() < ((uint64_t)lastCelOffset + 4 + (uint64_t)lastCelFrameCount * 4 + 4))
		{
			return;
		}

		fileStream.seek((uint64_t)lastCelOffset + 4 + (uint64_t)lastCelFrameCount * 4);
		fileStream.read(lastCelSize);

		// If the last CEL size plus the last CEL offset is equal to
		// the file size then it's a CEL compilation
		if (fileData.size() == (lastCelOffset + lastCelSize))
		{
			type = CelType::V1Compilation;
			groupCount = firstDword / 4;
		}
		else
		{
			return;
		}
	}
	else
	{
		type = CelType::V1Regular;
		groupCount = 1;
	}

	// CEL FRAMES OFFSETS CALCULATION

	if (type == CelType::V1Compilation)
	{
		// Going through all CELs
		for (size_t i = 0; i * 4 < firstDword; i++)
		{
			fileStream.seek((uint64_t)i * 4);
			fileStream.read(celOffset);

			fileStream.seek(celOffset);
			fileStream.read(celFrameCount);

			groupFrameIndexes.push_back(
				std::make_pair((uint16_t)frameOffsets.size(),
				(uint16_t)(frameOffsets.size() + celFrameCount - 1)));

			// Going through all frames of the CEL
			for (size_t j = 1; j <= celFrameCount; j++)
			{
				celFrameStartOffset = 0;
				celFrameEndOffset = 0;

				fileStream.seek((uint64_t)celOffset + (uint64_t)j * 4);
				fileStream.read(celFrameStartOffset);
				fileStream.read(celFrameEndOffset);

				frameOffsets.push_back(
					std::make_pair(celOffset + celFrameStartOffset,
						celOffset + celFrameEndOffset));
			}
		}
	}
	else
	{
		uint32_t level0x400FrameCount = 0;
		uint32_t level0x320FrameCount = 0;
		uint32_t level0x220FrameCount = 0;

		// Going through all frames of the CEL
		for (size_t i = 1; i <= firstDword; i++)
		{
			celFrameStartOffset = 0;
			celFrameEndOffset = 0;

			fileStream.seek((uint64_t)i * 4);
			fileStream.read(celFrameStartOffset);
			fileStream.read(celFrameEndOffset);

			frameOffsets.push_back(std::make_pair(celFrameStartOffset, celFrameEndOffset));

			// Level CEL Check
			celFrameSize = celFrameEndOffset - celFrameStartOffset;
			if (celFrameSize == 0x400)
			{
				level0x400FrameCount++;
			}
			else if (celFrameSize == 0x320)
			{
				level0x320FrameCount++;
			}
			else if (celFrameSize == 0x220)
			{
				level0x220FrameCount++;
			}
		}
		// If there are more than 512 frames of 0x400, 0x320 and 0x220 size,
		// then it's a level frame.
		if (level0x400FrameCount + level0x320FrameCount + level0x220FrameCount > 512)
		{
			type = CelType::V1Level;
		}
	}
}

sf::Image2 CelImageContainer::get(size_t index, const PaletteArray* palette) const
{
	if (index >= frameOffsets.size())
	{
		return {};
	}

	auto frameSize = frameOffsets[index].second - frameOffsets[index].first;
	gsl::span<const uint8_t> frameData(&fileData[frameOffsets[index].first], frameSize);

	// If it's not a level CEL
	if (type != CelType::V1Level)
	{
		return decode(frameData, 0, 0, CelFrameType::Regular, palette);
	}
	// If it's a level CEL
	else
	{
		auto frameType = CelFrameType::Regular;
		switch (frameSize)
		{
		case 0x400:
			frameType = getLevelFrame400Type(frameData);
			break;
		case 0x320:
			frameType = getLevelFrame320Type(frameData);
			break;
		case 0x220:
			frameType = getLevelFrame220Type(frameData);
			break;
		default:
			break;
		}
		return decode(frameData, 32, 32, frameType, palette);
	}
}
