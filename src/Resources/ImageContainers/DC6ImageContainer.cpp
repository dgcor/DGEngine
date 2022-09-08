#include "DC6ImageContainer.h"
#include <span>
#include "Utils/StreamReader.h"

namespace
{
	constexpr static auto DC6FrameHeaderSize = 32;
	constexpr static auto DC6MaxFrameSize = 256;

	struct DC6FrameHeader
	{
		int32_t flip;		// Default (false) means scanlines are from bottom to top
		uint32_t width;		// Width in pixels
		uint32_t height;	// Height in pixels
		int32_t offsetX;	// Horizontal offset for left edge of the frame
		int32_t offsetY;	// Vertical offset for bottom (top if flipped) edge of the frame.
		int32_t allocSize;	// Used by the game as a slot to store the data size. 0 in the files.
		int32_t nextBlock;	// Offset/Pointer to the next frame
		uint32_t length;	// Length of the frame in chunks
	};

	bool decodeFrameHeader(uint32_t index, const std::vector<uint8_t>& fileData,
		DC6FrameHeader& frameHeader, std::span<const uint8_t>& frameData)
	{
		// get frame position
		auto framePosIdx = 0x18 + (index * sizeof(uint32_t));
		if (framePosIdx >= fileData.size())
		{
			return false;
		}

		auto framePos = &fileData[framePosIdx];

		uint32_t frameHeaderPos = 0;
		endian::little_endian::get(frameHeaderPos, framePos);

		if (frameHeaderPos >= fileData.size() - DC6FrameHeaderSize)
		{
			return false;
		}

		{
			LittleEndianStreamReader frameHeaderStream(
				&fileData[frameHeaderPos], DC6FrameHeaderSize
			);
			frameHeaderStream.read(frameHeader.flip);
			frameHeaderStream.read(frameHeader.width);
			frameHeaderStream.read(frameHeader.height);
			frameHeaderStream.read(frameHeader.offsetX);
			frameHeaderStream.read(frameHeader.offsetY);
			frameHeaderStream.read(frameHeader.allocSize);
			frameHeaderStream.read(frameHeader.nextBlock);
			frameHeaderStream.read(frameHeader.length);
		}

		if (frameHeader.width == 0 || frameHeader.height == 0)
		{
			return false;
		}

		frameData = std::span<const uint8_t>(
			&fileData[frameHeaderPos] + DC6FrameHeaderSize, frameHeader.length
		);
		return true;
	}

	void decodeFrameData(sf::Image2& img,
		const DC6FrameHeader& header, const std::span<const uint8_t>& frameData,
		uint32_t destX, uint32_t destY, const PaletteArray* palette)
	{
		// We're reading it bottom to top, but save data with the y axis from top to bottom
		uint32_t x = 0;
		uint32_t y = (header.flip == 0 ? header.height - 1 : 0);
		uint32_t dataIndex = 0;
		while (dataIndex < header.length)
		{
			auto readByte = frameData[dataIndex];
			dataIndex++;

			// end of line
			if (readByte == 0x80)
			{
				x = 0;
				if (header.flip == 0)
				{
					y--;
				}
				else
				{
					y++;
				}
			}
			// transparent pixels
			else if (readByte & 0x80)
			{
				x += readByte & 0x7F;
			}
			// color pixels
			else
			{
				for (uint32_t i = 0; i < readByte; i++)
				{
					auto color = ImageContainer::getColor(frameData[dataIndex++], palette);
					img.setPixel(destX + x, destY + y, color);
					x++;
				}
			}
		}
	}
}

DC6ImageContainer::DC6ImageContainer(const std::shared_ptr<FileBytes>& fileBytes, bool stitchFrames, bool useOffsets_)
	: fileData(fileBytes), useOffsets(useOffsets_)
{
	LittleEndianStreamReader fileStream(fileData->data(), fileData->size());

	// DC6 HEADER CHECKS

	// version
	int32_t version = 0;
	fileStream.read(version);
	if (version != 6)
	{
		return;
	}

	// flags
	int32_t flags = 0;
	fileStream.read(flags);
	if (flags != 1)
	{
		return;
	}

	// format
	int32_t format = 0;
	fileStream.read(format);
	if (format != 0)
	{
		return;
	}

	// marker
	uint32_t marker = 0;
	fileStream.read(marker);
	if ((marker == 0xEEEEEEEE || marker == 0xCDCDCDCD || marker == 0) == false)
	{
		return;
	}

	// directions and frames per direction
	fileStream.seek(0x10);
	uint32_t framesPerDir{ 0 };
	fileStream.read(directions);
	fileStream.read(framesPerDir);
	numberOfFrames = directions * framesPerDir;

	if (stitchFrames == true && directions == 1 && framesPerDir > 1)
	{
		calculateStitchData();
	}
}

void DC6ImageContainer::calculateStitchData()
{
	bool stitching = false;
	StitchedFrame stitchedFrame;

	for (uint32_t i = 0; i < numberOfFrames; i++)
	{
		DC6FrameHeader frameHeader;
		std::span<const uint8_t> frameData;
		if (decodeFrameHeader(i, *fileData, frameHeader, frameData) == false)
		{
			return;
		}

		if (stitching == false)
		{
			if (frameHeader.width == DC6MaxFrameSize ||
				frameHeader.height == DC6MaxFrameSize)
			{
				stitching = true;
				stitchedFrame.startIndex = i;
				stitchedFrame.stitch.y++;
			}
			else
			{
				return;
			}
		}

		if (stitching == true)
		{
			stitchedFrame.stitch.x++;
			stitchedFrame.size.x += frameHeader.width;

			if (frameHeader.width != DC6MaxFrameSize)
			{
				while (frameHeader.height == DC6MaxFrameSize)
				{
					i += stitchedFrame.stitch.x;
					stitchedFrame.stitch.y++;
					stitchedFrame.size.y += frameHeader.height;
					if (decodeFrameHeader(i, *fileData, frameHeader, frameData) == false)
					{
						return;
					}
				}
				stitchedFrame.size.y += frameHeader.height;

				stitchedFrames.push_back(stitchedFrame);

				stitching = false;
				stitchedFrame = {};
			}
			continue;
		}
		break;
	}
}

sf::Image2 DC6ImageContainer::get(uint32_t startIndex, const sf::Vector2u& stitch_,
	const sf::Vector2u& size_, const PaletteArray* palette) const
{
	sf::Image2 img;
	img.create(size_.x, size_.y, sf::Color::Transparent);

	uint32_t destY = 0;

	if (stitch_.x > 0 && stitch_.y > 0 &&
		startIndex + (stitch_.x * stitch_.y) <= numberOfFrames)
	{
		for (uint32_t j = 0; j < stitch_.y; j++)
		{
			uint32_t destX = 0;
			uint32_t maxDestY = 0;
			for (uint32_t i = 0; i < stitch_.x; i++)
			{
				DC6FrameHeader frameHeader;
				std::span<const uint8_t> frameData;
				auto frameIdx = startIndex + i + (j * stitch_.x);
				if (decodeFrameHeader(frameIdx, *fileData, frameHeader, frameData) == false)
				{
					return img;
				}
				decodeFrameData(img, frameHeader, frameData, destX, destY, palette);

				destX += frameHeader.width;
				maxDestY = std::max(maxDestY, frameHeader.height);
			}
			destY += maxDestY;
		}
	}
	return img;
}

sf::Image2 DC6ImageContainer::get(const sf::Vector2u& stitch_,
	const sf::Vector2u& size_, const PaletteArray* palette) const
{
	return get(0, stitch_, size_, palette);
}

sf::Image2 DC6ImageContainer::get(uint32_t index,
	const PaletteArray* palette, ImageInfo& imgInfo) const
{
	if (index >= size())
	{
		return {};
	}

	if (stitchedFrames.empty() == false)
	{
		imgInfo.offset = {};
		imgInfo.absoluteOffset = false;
		imgInfo.blendMode = blendMode;
		const auto& stitchedFrame = stitchedFrames[index];
		return get(
			stitchedFrame.startIndex,
			stitchedFrame.stitch,
			stitchedFrame.size,
			palette
		);
	}

	DC6FrameHeader frameHeader;
	std::span<const uint8_t> frameData;
	if (decodeFrameHeader(index, *fileData, frameHeader, frameData) == false)
	{
		return {};
	}

	if (useOffsets == true)
	{
		imgInfo.offset.x = (float)frameHeader.offsetX;
		imgInfo.offset.y = (float)frameHeader.offsetY;
		if (frameHeader.flip == 0)
		{
			imgInfo.offset.y -= (float)frameHeader.height;
		}
		imgInfo.absoluteOffset = true;
	}
	else
	{
		imgInfo.offset = {};
		imgInfo.absoluteOffset = false;
	}
	imgInfo.blendMode = blendMode;
	imgInfo.nextIndex = -1;

	sf::Image2 img;
	img.create(frameHeader.width, frameHeader.height, sf::Color::Transparent);

	decodeFrameData(img, frameHeader, frameData, 0, 0, palette);

	return img;
}

uint32_t DC6ImageContainer::size() const noexcept
{
	return stitchedFrames.empty() == true ? numberOfFrames : (uint32_t)stitchedFrames.size();
}
