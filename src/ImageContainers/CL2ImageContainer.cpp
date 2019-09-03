#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "CL2ImageContainer.h"
#include "gsl/gsl"
#include "PhysFSStream.h"
#include "StreamReader.h"

namespace
{
	enum class CelType
	{
		None,
		V2MonoGroup,
		V2MultipleGroups
	};

	uint16_t computeWidthFromHeader(const gsl::span<const uint8_t> frameData)
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

				if (readByte > 0x00 && readByte < 0x80)
				{
					pixelCount += readByte;
				}
				else if (readByte >= 0x80 && readByte < 0xBF)
				{
					pixelCount += (0xBFu - readByte);
					j++;
				}
				else if (readByte >= 0xBF)
				{
					pixelCount += (256u - readByte);
					j += (256u - readByte);
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

	sf::Image2 decode(const gsl::span<const uint8_t> frameData, const PaletteArray* palette)
	{
		unsigned width;
		int32_t frameDataStartOffset = 0;

		if (frameData.size() == 0)
		{
			return {};
		}

		// Checking the presence of the {CL2 FRAME HEADER}
		if (frameData[0] == 0x0A && frameData[1] == 0x00)
		{
			frameDataStartOffset += 0x0A;
			// If header is present, try to compute frame width from frame header
			width = computeWidthFromHeader(frameData);
		}
		else
		{
			return {};
		}
		if (width == 0)
		{
			return {};
		}

		std::vector<sf::Color> pixels;
		pixels.reserve(width * width);

		// READ {CL2 FRAME DATA}

		for (ptrdiff_t i = frameDataStartOffset; i < frameData.size(); i++)
		{
			auto readByte = frameData[i];

			// Transparent pixels
			if (readByte > 0x00 && readByte < 0x80)
			{
				for (size_t j = 0; j < readByte; j++)
				{
					// Add transparent pixel
					pixels.push_back(sf::Color::Transparent);
				}
			}
			// Repeat palette index
			else if (readByte >= 0x80 && readByte < 0xBF)
			{
				// Go to the palette index offset
				i++;

				for (size_t j = 0; j < (0xBFu - readByte); j++)
				{
					// Add opaque pixel
					pixels.push_back(ImageContainer::getColor(frameData[i], palette));
				}
			}
			// Palette indices
			else if (readByte >= 0xBF)
			{
				for (size_t j = 0; j < (256u - readByte); j++)
				{
					// Go to the next palette index offset
					i++;
					// Add opaque pixel
					pixels.push_back(ImageContainer::getColor(frameData[i], palette));
				}
			}
			else if (readByte == 0x00)
			{
				// 0x00 found
			}
		}
		unsigned height = pixels.size() / width;

		sf::Image2 img;
		img.create(width, height, (const sf::Uint8*)pixels.data());
		img.flipVertically();

		return img;
	}
}

CL2ImageContainer::CL2ImageContainer(const std::string_view fileName)
{
	CelType type = CelType::V2MultipleGroups;
	uint32_t firstDword = 0;
	uint32_t fileSizeDword = 0;

	{
		sf::PhysFSStream file(fileName.data());
		if (file.hasError() == true)
		{
			return;
		}
		fileData.resize((size_t)file.getSize());
		file.read(fileData.data(), file.getSize());
	}

	LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

	// CL2 HEADER CHECKS

	// Read first DWORD
	fileStream.read(firstDword);

	// Trying to find file size in CL2 header
	if (fileData.size() < ((uint64_t)firstDword * 4 + 4 + 4))
	{
		return;
	}

	fileStream.seek((uint64_t)firstDword * 4 + 4);
	fileStream.read(fileSizeDword);

	// If the dword is not equal to the file size then
	// check if it's a CL2 with multiple groups
	if (fileData.size() != fileSizeDword)
	{
		// Read offset of the last CL2 group header
		uint32_t lastGroupHeaderOffset;
		fileStream.seek(firstDword - 4);
		fileStream.read(lastGroupHeaderOffset);

		// Read the number of frames of the last CL2 group
		if (fileData.size() < lastGroupHeaderOffset)
		{
			return;
		}

		uint32_t lastGroupFrameCount;
		fileStream.seek(lastGroupHeaderOffset);
		fileStream.read(lastGroupFrameCount);

		// Read the last frame offset corresponding to the file size
		if (fileData.size() < lastGroupHeaderOffset + lastGroupFrameCount * 4 + 4 + 4)
		{
			return;
		}

		fileStream.seek((uint64_t)lastGroupHeaderOffset + (uint64_t)lastGroupFrameCount * 4 + 4);
		fileStream.read(fileSizeDword);
		// The offset is from the beginning of the last group header
		// so we need to add the offset of the lasr group header
		// to have an offset from the beginning of the file
		fileSizeDword += lastGroupHeaderOffset;

		if (fileData.size() == fileSizeDword)
		{
			type = CelType::V2MultipleGroups;
			directions = firstDword / 4;
		}
		else
		{
			return;
		}
	}
	else
	{
		type = CelType::V2MonoGroup;
		directions = 1;
	}

	// CL2 FRAMES OFFSETS CALCULATION

	frameOffsets.clear();
	if (type == CelType::V2MultipleGroups)
	{
		// Going through all groups
		for (uint32_t i = 0; i * 4 < firstDword; i++)
		{
			uint32_t groupOffset;
			uint32_t groupFrameCount;

			fileStream.seek((uint64_t)i * 4);
			fileStream.read(groupOffset);

			fileStream.seek(groupOffset);
			fileStream.read(groupFrameCount);

			// Going through all frames of the group
			for (uint32_t j = 1; j <= groupFrameCount; j++)
			{
				uint32_t frameStartOffset = 0;
				uint32_t frameEndOffset = 0;

				fileStream.seek((uint64_t)groupOffset + (uint64_t)j * 4);
				fileStream.read(frameStartOffset);
				fileStream.read(frameEndOffset);

				frameOffsets.push_back(
					std::make_pair(groupOffset + frameStartOffset,
						groupOffset + frameEndOffset));
			}
		}
	}
	else
	{
		// Going through all frames of the only group
		for (size_t i = 1; i <= firstDword; i++)
		{
			uint32_t frameStartOffset;
			uint32_t frameEndOffset;

			fileStream.seek((uint64_t)i * 4);
			fileStream.read(frameStartOffset);
			fileStream.read(frameEndOffset);

			frameOffsets.push_back(std::make_pair(frameStartOffset, frameEndOffset));
		}
	}
}

sf::Image2 CL2ImageContainer::get(uint32_t index,
	const PaletteArray* palette, ImageInfo& imgInfo) const
{
	if (index >= frameOffsets.size())
	{
		return {};
	}

	imgInfo.offset = {};
	imgInfo.absoluteOffset = false;
	imgInfo.blendMode = blendMode;
	imgInfo.nextIndex = -1;

	uint32_t frameSize = frameOffsets[index].second - frameOffsets[index].first;
	gsl::span<const uint8_t> frameData(&fileData[frameOffsets[index].first], frameSize);
	return decode(frameData, palette);
}
#endif
