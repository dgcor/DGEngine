#include "Cel.h"
#include <iostream>
#include "PhysFSStream.h"

CelFrame::operator sf::Image() const
{
	sf::Image img;
	img.create(width, height, sf::Color::Transparent);

	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++)
		{
			img.setPixel(i, j, (*this)[i][j]);
		}
	}
	return img;
}

CelFrame::operator sf::Texture() const
{
	sf::Image img = *this;
	auto tex = sf::Texture();
	tex.loadFromImage(img);
	return tex;
}

//begin cel
int32_t normalWidth(const std::vector<uint8_t>& frame, size_t frameNum, bool fromHeader, uint16_t offset)
{
	// If we have a header, we know that offset points to the end of the 32nd line.
	// So, when we reach that point, we will have produced 32 lines of pixels, so we 
	// can divide the number of pixels we have passed at this point by 32, to get the 
	// width.
	if (fromHeader)
	{
		// Workaround for objcurs.cel, the only cel file containing frames with a header whose offset is zero
		if (offset == 0)
		{
			if (frameNum == 0) {
				return 33;
			}
			else if (frameNum > 0 && frameNum < 10) {
				return 32;
			}
			else if (frameNum == 10) {
				return 23;
			}
			else if (frameNum > 10 && frameNum < 86) {
				return 28;
			}
		}

		int32_t widthHeader = 0;

		for (size_t i = 10; i < frame.size(); i++)
		{
			if (i == offset && fromHeader)
			{
				widthHeader = widthHeader / 32;
				break;
			}
			// Regular command
			if (frame[i] <= 127) {
				widthHeader += frame[i];
				i += frame[i];
			}
			else if (128 <= frame[i])	// Transparency command
			{
				widthHeader += 256 - frame[i];
			}
		}

		return widthHeader;
	}

	// If we do not have a header we probably (definitely?) don't have any transparency.
	// The maximum stretch of opaque pixels following a command byte is 127.
	// Since commands can't wrap over lines (it seems), if the width is shorter than 127,
	// the first (command) byte will indicate an entire line, so it's value is the width.
	// If the width is larger than 127, it will be some sequence of 127 byte long stretches,
	// followed by some other value to bring it to the end of a line (presuming the width is
	// not divisible by 127).
	// So, for all image except those whose width is divisible by 127, we can determine width
	// by looping through control bits, adding 127 each time, until we find some value which
	// is not 127, then add that to the 127-total and that is our width.
	//
	// The above is the basic idea, but there is also a bunch of crap added in to maybe deal
	// with frames that don't quite fit the criteria.
	else
	{
		int32_t widthRegular = 0;
		bool hasTrans = false;

		uint8_t lastVal = 0;
		uint8_t lastTransVal = 0;

		for (size_t i = 0; i < frame.size(); i++)
		{
			uint8_t val = frame[i];

			// Regular command
			if (val <= 127)
			{
				widthRegular += val;
				i += val;

				// Workaround for frames that start with a few px, then trans for the rest of the line
				if (128 <= frame[i + 1]) {
					hasTrans = true;
				}
			}
			else if (128 <= val)
			{
				// Workaround for frames that start trans, then a few px of colour at the end
				if (val == lastTransVal && lastVal <= 127 && lastVal == frame[i + 1]) {
					break;
				}

				widthRegular += 256 - val;

				// Workaround - presumes all headerless frames first lines start transparent, then go colour,
				// then go transparent again, at which point they hit the end of the line, or if the first two
				// commands are both transparency commands, that the image starts with a fully transparent line
				if ((hasTrans || 128 <= frame[i + 1]) && val != 128) {
					break;
				}

				hasTrans = true;

				lastTransVal = val;
			}

			if (val != 127 && !hasTrans) {
				break;
			}

			lastVal = val;
		}

		return widthRegular;
	}
}

int32_t normalDecode(const std::vector<uint8_t>& frame, size_t frameNum, const Palette& pal, std::vector<sf::Color>& rawImage, bool tileCel)
{
	size_t i = 0;

	uint16_t offset = 0;
	bool fromHeader = false;

	// The frame has a header which we can use to determine width
	if (!tileCel && frame[0] == 10)
	{
		fromHeader = true;
		offset = (uint16_t)(frame[3] << 8 | frame[2]);
		i = 10; // Skip the header
	}

	for (; i < frame.size(); i++)
	{
		// Regular command
		if (frame[i] <= 127)
		{
			size_t j;
			// Just push the number of pixels specified by the command
			for (j = 1; j < frame[i] + 1 && i + j < frame.size(); j++)
			{
				int index = i + j;
				uint8_t f = frame[index];

				if (index > frame.size() - 1) {
					std::cout << "invalid read from f " << index << " " << frame.size() << std::endl;
				}

				rawImage.push_back(pal[(size_t)f]);
			}

			i += frame[i];
		}
		else if (128 <= frame[i])	// Transparency command
		{
			// Push (256 - command value) transparent pixels
			for (size_t j = 0; j < 256 - frame[i]; j++) {
				rawImage.push_back(sf::Color::Transparent);
			}
		}
	}

	return normalWidth(frame, frameNum, fromHeader, offset);
}
//end cel

//begin cl2
int32_t cl2Width(const std::vector<uint8_t>& frame, uint16_t offset)
{
	int32_t pixels = 0;

	size_t i = 10; // CL2 frames always have headers

	for (; i < frame.size(); i++)
	{
		if (i == offset) {
			return pixels / 32;
		}

		// Color command
		if (frame[i] > 127)
		{
			uint8_t val = 256 - frame[i];

			// Regular command
			if (val <= 65)
			{
				pixels += val;
				i += val;
			}
			else	// RLE (run length encoded) Colour command
			{
				pixels += val - 65;
				i += 1;
			}
		}
		else	// Transparency command
		{
			pixels += frame[i];
		}
	}

	return -1; // keep the compiler happy
}

int32_t cl2Decode(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage)
{
	size_t i = 10; // CL2 frames always have headers

	for (; i < frame.size(); i++)
	{
		// Color command
		if (frame[i] > 127)
		{
			uint8_t val = 256 - frame[i];

			// Regular command
			if (val <= 65)
			{
				size_t j;
				// Just push the number of pixels specified by the command
				for (j = 1; j < val + 1 && i + j < frame.size(); j++)
				{
					int index = i + j;
					auto f = frame[index];

					if (index > frame.size() - 1) {
						std::cout << "invalid read from f " << index << " " << frame.size() << std::endl;
					}

					rawImage.push_back(pal[f]);
				}

				i += val;
			}
			else	// RLE (run length encoded) Colour command
			{
				for (int j = 0; j < val - 65; j++) {
					rawImage.push_back(pal[frame[i + 1]]);
				}

				i += 1;
			}
		}
		else	// Transparency command
		{
			// Push transparent pixels
			for (size_t j = 0; j < frame[i]; j++) {
				rawImage.push_back(sf::Color::Transparent);
			}
		}
	}

	uint16_t offset = (uint16_t)(frame[3] << 8 | frame[2]);
	return cl2Width(frame, offset);
}
// end cl2

//begin tile
bool greaterThanFirst(const std::vector<uint8_t>& frame)
{
	return frame.size() >= 196 &&
		frame[2] == 0 && frame[3] == 0 &&
		frame[14] == 0 && frame[15] == 0 &&
		frame[34] == 0 && frame[35] == 0 &&
		frame[62] == 0 && frame[63] == 0 &&
		frame[98] == 0 && frame[99] == 0 &&
		frame[142] == 0 && frame[143] == 0 &&
		frame[194] == 0 && frame[195] == 0;
}

bool greaterThanSecond(const std::vector<uint8_t>& frame)
{
	return frame.size() >= 196 &&
		frame[254] == 0 && frame[255] == 0 &&
		frame[318] == 0 && frame[319] == 0 &&
		frame[374] == 0 && frame[375] == 0 &&
		frame[422] == 0 && frame[423] == 0 &&
		frame[462] == 0 && frame[463] == 0 &&
		frame[494] == 0 && frame[495] == 0 &&
		frame[518] == 0 && frame[519] == 0 &&
		frame[534] == 0 && frame[535] == 0;
}

bool isGreaterThan(const std::vector<uint8_t>& frame)
{
	return greaterThanFirst(frame);
}

bool lessThanFirst(const std::vector<uint8_t>& frame)
{
	return frame.size() >= 226 &&
		frame[0] == 0 && frame[1] == 0 &&
		frame[8] == 0 && frame[9] == 0 &&
		frame[24] == 0 && frame[25] == 0 &&
		frame[48] == 0 && frame[49] == 0 &&
		frame[80] == 0 && frame[81] == 0 &&
		frame[120] == 0 && frame[121] == 0 &&
		frame[168] == 0 && frame[169] == 0 &&
		frame[224] == 0 && frame[225] == 0;
}

bool lessThanSecond(const std::vector<uint8_t>& frame)
{
	return frame.size() >= 530 &&
		frame[288] == 0 && frame[289] == 0 &&
		frame[348] == 0 && frame[349] == 0 &&
		frame[400] == 0 && frame[401] == 0 &&
		frame[444] == 0 && frame[445] == 0 &&
		frame[480] == 0 && frame[481] == 0 &&
		frame[508] == 0 && frame[509] == 0 &&
		frame[528] == 0 && frame[529] == 0;
}

bool isLessThan(const std::vector<uint8_t>& frame)
{
	return lessThanFirst(frame);
}

void fillTransparent(size_t pixels, std::vector<sf::Color>& rawImage)
{
	for (int px = 0; px < pixels; px++) {
		rawImage.push_back(sf::Color(255, 255, 255, false));
	}
}

void drawRow(int row, int lastRow, int& framePos, const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage, bool lessThan)
{
	for (; row < lastRow; row++)
	{
		// Skip markers - for less than, when on the first half of the image (row < 16), all even rows will start with a pair of marker bits
		// for the second half of the image (row >= 16), all odd rows will start with a pair of marker bits.
		// The inverse is true of greater than images.
		if ((lessThan && ((row < 16 && row % 2 == 0) || (row >= 16 && row % 2 != 0))) ||
			(!lessThan && ((row < 16 && row % 2 != 0) || (row >= 16 && row % 2 == 0))))
		{
			framePos += 2;
		}

		int toDraw;
		if (row < 16) {
			toDraw = 2 + (row * 2);
		}
		else {
			toDraw = 32 - ((row - 16) * 2);
		}

		if (lessThan) {
			fillTransparent(32 - toDraw, rawImage);
		}

		for (int px = 0; px < toDraw; px++)
		{
			rawImage.push_back(pal[frame[framePos]]);
			framePos++;
		}

		if (!lessThan) {
			fillTransparent(32 - toDraw, rawImage);
		}
	}
}

void decodeGreaterLessThan(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage, bool lessThan)
{
	int framePos = 0;

	drawRow(0, 15, framePos, frame, pal, rawImage, lessThan);

	if ((lessThan && lessThanSecond(frame)) || (!lessThan && greaterThanSecond(frame)))
	{
		drawRow(16, 33, framePos, frame, pal, rawImage, lessThan);
	}
	else
	{
		for (framePos = 256; framePos < frame.size(); framePos++) {
			rawImage.push_back(pal[frame[framePos]]);
		}
	}
}

void decodeGreaterThan(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage)
{
	decodeGreaterLessThan(frame, pal, rawImage, false);
}

void decodeLessThan(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage)
{
	decodeGreaterLessThan(frame, pal, rawImage, true);
}

size_t decodeRaw32(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage)
{
	for (int i = 0; i < frame.size(); i++)
	{
		rawImage.push_back(pal[frame[i]]);
	}

	return 32;
}

size_t decodeTileFrame(const std::vector<uint8_t>& frame, const Palette& pal, std::vector<sf::Color>& rawImage)
{
	if (frame.size() == 1024 /*&& frame_num != 2593*/) { // It's a fully opaque raw frame, width 32, from a level tileset
		decodeRaw32(frame, pal, rawImage);
	}

	else if (isLessThan(frame)) {
		decodeLessThan(frame, pal, rawImage);
	}

	else if (isGreaterThan(frame)) {
		decodeGreaterThan(frame, pal, rawImage);
	}
	else {
		normalDecode(frame, 0, pal, rawImage, true); // pass zero as frameNum because it's only used for width calculation and width of tile frames is always 32
	}

	return 32;
}
//end tile
//begin celfile
CelFile::CelFile(const char* filename, bool isCl2_, bool isTileCel_)
	: isCl2(isCl2_), isTileCel(isTileCel_)
{
	sf::PhysFSStream file(filename);

	uint32_t first;
	file.read(&first, 4);

	// If the first uint16_t in the file is 32,
	// then it is a cel archive, containing 8 cels,
	// each of which is a collection of frames 
	// representing an animation of an object at 
	// one of the eight possible rotations.
	// This is a side effect of cel archives containing
	// a header like the normal cel header pointing to
	// each of the cels it contains, and there always being
	// 8 cels in each cel archive, so 8*4=32, the start
	// of the first cel
	if (first == 32)
	{
		if (isCl2 == true)
		{
			animLength = readCl2ArchiveFrames(file);
		}
		else
		{
			file.seek(32);
			for (size_t i = 0; i < 8; i++) {
				animLength = readNormalFrames(file);
			}
		}
	}
	else
	{
		file.seek(0);
		animLength = readNormalFrames(file);
	}
}

CelFrame CelFile::get(size_t index, const Palette& palette) const
{
	std::vector<sf::Color> rawImage;
	auto width = getFrame(mFrames[index], palette, index, rawImage);
	auto height = rawImage.size() / width;

	return CelFrame(rawImage, width, height);
}

size_t CelFile::getFrame(const std::vector<uint8_t>& frame, const Palette& palette,
	size_t frameNum, std::vector<sf::Color>& rawImage) const
{
	if (isCl2 == true)
	{
		return cl2Decode(frame, palette, rawImage);
	}
	if (isTileCel == true)
	{
		return decodeTileFrame(frame, palette, rawImage);
	}
	return normalDecode(frame, frameNum, palette, rawImage, false);
}

size_t CelFile::readCl2ArchiveFrames(sf::InputStream& file)
{
	file.seek(0);

	std::vector<uint32_t> headerOffsets(8);
	file.read(&headerOffsets[0], 32);

	uint32_t numFrames;

	for (size_t i = 0; i < 8; i++)
	{
		file.seek(headerOffsets[i]);
		file.read(&numFrames, 4);

		std::vector<uint32_t> frameOffsets(numFrames + 1);


		for (size_t j = 0; j <= numFrames; j++)
		{
			file.read(&frameOffsets[j], 4);
		}

		file.seek(headerOffsets[i] + frameOffsets[0]);

		for (size_t j = 0; j < numFrames; j++)
		{
			mFrames.push_back(std::vector<uint8_t>(frameOffsets[j + 1] - frameOffsets[j]));

			file.read(&mFrames[mFrames.size() - 1][0], frameOffsets[j + 1] - frameOffsets[j]);
		}
	}

	return numFrames;
}

size_t CelFile::readNormalFrames(sf::InputStream& file)
{
	uint32_t numFrames;

	file.read(&numFrames, 4);

	std::vector<uint32_t> frameOffsets(numFrames + 1);

	for (size_t i = 0; i < numFrames; i++)
	{
		file.read(&frameOffsets[i], 4);
	}

	file.read(&frameOffsets[numFrames], 4);

	for (size_t i = 0; i < numFrames; i++)
	{
		mFrames.push_back(std::vector<uint8_t>(frameOffsets[i + 1] - frameOffsets[i]));

		file.read(&mFrames[mFrames.size() - 1][0], frameOffsets[i + 1] - frameOffsets[i]);
	}

	return numFrames;
}
//end celfile
