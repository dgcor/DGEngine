#include "DCCImageContainer.h"
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include "StreamReader.h"

namespace
{
	struct ImageView
	{
		uint8_t* buffer = nullptr;	// Pointer to the memory considered as the first pixel
		size_t width = 0;	// Width of the image, can be different from the one of the buffer
		size_t height = 0;	// Number of scanlines of the image
		size_t stride = 0;	// Actual width of the buffer scanlines

		ImageView() = default;

		ImageView(uint8_t* _buffer, size_t _width, size_t _height, size_t _stride)
			: buffer(_buffer), width(_width), height(_height), stride(_stride)
		{
		}

		bool isValid() const { return buffer && width && height && width <= stride; }

		ImageView subView(size_t xOffset, size_t yOffset, size_t subWidth, size_t subHeight) const
		{
			if (xOffset + subWidth > width || yOffset + subHeight > height) return {};
			return { buffer + xOffset + yOffset * stride, subWidth, subHeight, stride };
		}

		uint8_t& operator()(size_t x, size_t y) const { return buffer[x + y * stride]; }

		void copyTo(ImageView destination) const
		{
			for (size_t y = 0; y < height; y++)
			{
				memcpy(destination.buffer + y * destination.stride, buffer + y * stride,
					width * sizeof(uint8_t));
			}
		}

		void fillBytes(size_t x, size_t y, size_t columns, size_t rows, uint8_t byteValue)
		{
			uint8_t* dstPtr = buffer + x + y * stride;
			for (size_t row = 0; row < rows; ++row)
			{
				memset(dstPtr, byteValue, columns * sizeof(uint8_t));
				dstPtr += stride;
			}
		}
	};

	class SimpleImageProvider
	{
		struct Image
		{
			size_t width = 0;
			size_t height = 0;
			std::vector<uint8_t> buffer;
			Image(size_t _width, size_t _height)
				: width(_width), height(_height), buffer(_width* _height)
			{
			}
		};
		std::vector<Image> images;

	public:
		ImageView getNewImage(size_t width, size_t height)
		{
			if (!width || !height) return {};
			images.emplace_back(width, height);
			return { images.back().buffer.data(), width, height, width };
		}

		size_t getImagesNumber() const { return images.size(); }

		ImageView getImage(size_t imageIndex) const
		{
			const Image& img = images[imageIndex];
			return { (uint8_t*)img.buffer.data(), img.width, img.height, img.width };
		}
	};

	// Axis-Aligned Bounding Box
	template<typename T>
	struct AABB
	{
		T xLower; // x lower bound : excluded
		T yLower; // y lower bound : excluded
		T xUpper; // x upper bound : excluded
		T yUpper; // y upper bound : excluded

		T width() const { return xUpper - xLower; }
		T height() const { return yUpper - yLower; }

		void maximize()
		{
			xLower = std::numeric_limits<T>::lowest();
			yLower = std::numeric_limits<T>::lowest();
			xUpper = std::numeric_limits<T>::max();
			yUpper = std::numeric_limits<T>::max();
		}

		void initializeForExtension() { *this = AABB::getInitializedForExtension(); }

		void extend(const AABB& other)
		{
			xLower = std::min(xLower, other.xLower);
			yLower = std::min(yLower, other.yLower);
			xUpper = std::max(xUpper, other.xUpper);
			yUpper = std::max(yUpper, other.yUpper);
		}

		static constexpr AABB getInitializedForExtension()
		{
			return { std::numeric_limits<T>::max(), std::numeric_limits<T>::max(),
					std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest() };
		}
	};

	class BitStreamView
	{
	protected:
		static constexpr int goodbit = 0x0;
		static constexpr int eofbit = 0x1;
		static constexpr int failbit = 0x2;
		static constexpr int badbit = 0x4;
		int _state = goodbit;

		void setstate(int state) { _state = _state | state; }

	private:
		size_t size = 0;				// Size of the bitstream in bits
		size_t firstBitOffset = 0;		// Position of the first bit in the first byte
		size_t currentBitPosition = 0;	// Current absolute position in the buffer, in bits
		const uint8_t* buffer = nullptr;

		template<typename SignedResult, unsigned NbBits, typename InputType>
		inline SignedResult signExtend(const InputType value)
		{
			static_assert(std::is_integral<SignedResult>::value && std::is_integral<InputType>::value, "");
			static_assert(NbBits > 0, "Can not represent a signed value on 0 bit.");
			static_assert(NbBits <= sizeof(SignedResult) * 8,
				"Result type is not big enough to hold the values.");
			static_assert(std::is_signed<SignedResult>::value, "Result type must be signed");
			struct
			{
				SignedResult x : NbBits;
			} s;
			return s.x = static_cast<SignedResult>(value);
		}

	public:
		BitStreamView() = default;

		BitStreamView(const void* inputBuffer, size_t sizeInBits,
			size_t firstBitOffsetInBuffer = 0) : size(sizeInBits),
			firstBitOffset(firstBitOffsetInBuffer),
			currentBitPosition(firstBitOffsetInBuffer),
			buffer(static_cast<const uint8_t*>(inputBuffer))
		{
			assert(firstBitOffset + size <= bufferSizeInBits());
		}

		explicit operator bool() const { return !fail(); }
		bool operator!() const { return fail(); }

		bool good() const { return _state == goodbit; }
		bool eof() const { return _state & eofbit; }
		bool fail() const { return (_state & (badbit | failbit)) != 0; }
		bool bad() const { return (_state & badbit) != 0; }

		BitStreamView createSubView(size_t newbufferSizeInBits) const
		{
			// 0Bits is a special case as it should always have a size of 0
			if (newbufferSizeInBits == 0) return {};
			const size_t curBytesPos = currentBitPosition / 8;
			const size_t bitPosInCurByte = currentBitPosition % 8;
			assert(tell() + newbufferSizeInBits <= size);
			assert(currentBitPosition + newbufferSizeInBits <= bufferSizeInBits());
			return { buffer + curBytesPos, newbufferSizeInBits, bitPosInCurByte };
		}

		size_t tell() const { return currentBitPosition - firstBitOffset; }

		void setPosition(size_t newPosition)
		{
			assert(newPosition >= (size_t)0 && newPosition < size);
			currentBitPosition = newPosition + firstBitOffset;
		}

		size_t bitPositionInBuffer() const { return currentBitPosition; }

		void skip(size_t nbBits)
		{
			assert(currentBitPosition + nbBits < bufferSizeInBits());
			currentBitPosition += nbBits;
		}

		size_t bufferSizeInBytes() const { return (size + firstBitOffset + 7) / 8; }

		size_t bufferSizeInBits() const { return bufferSizeInBytes() * 8; }

		size_t sizeInBits() const { return size; }

		// Reads a single bit from the stream
		bool readBool()
		{
			const size_t currentBytesPos = currentBitPosition / 8;
			const size_t bitPosInCurrentByte = currentBitPosition % 8;
			const int mask = (1 << bitPosInCurrentByte);
			currentBitPosition++;
			return (buffer[currentBytesPos] & mask) == mask;
		}

		template<typename RetType = uint32_t>
		RetType readUnsigned(unsigned nbBits)
		{
			static_assert(std::is_unsigned<RetType>::value, "You must return an unsigned type !");
			RetType value = 0;
			size_t curBytesPos = currentBitPosition / 8;
			size_t bitPosInCurByte = currentBitPosition % 8;
			currentBitPosition += nbBits;
			// The following variable is needed because we are reading a little endian input.
			size_t curDestBitPosition = 0;
			while (curDestBitPosition < nbBits)
			{
				// How many bits we can read in this byte ?
				const size_t bitsToReadInCurByte =
					std::min(8 - bitPosInCurByte, nbBits - curDestBitPosition);
				const RetType mask = RetType((1U << bitsToReadInCurByte) - 1U);
				// The bits we got from the current byte
				const RetType inBits = (RetType(buffer[curBytesPos++]) >> bitPosInCurByte) & mask;
				// Place them at the right position
				value |= inBits << curDestBitPosition;
				curDestBitPosition += bitsToReadInCurByte;
				// Next time we start at the beginning of the byte, perhaps we could optimize by
				// Treating the first byte as a special case ?
				bitPosInCurByte = 0;
			}
			return value;
		}

		uint8_t readUnsigned8OrLess(const int nbBits)
		{
			const size_t curBytesPos = currentBitPosition / 8;
			const int bitPosInCurByte = currentBitPosition % 8;
			currentBitPosition += size_t(nbBits);
			// Note: Could get rid of the condition by having one extra byte at the end of the stream
			const uint16_t shortFromBuffer =
				buffer[curBytesPos] |
				((bitPosInCurByte + nbBits > 8) ? uint16_t(buffer[curBytesPos + 1] << 8) : 0);
			const unsigned mask = 0xFF >> (8 - nbBits);
			const uint8_t value = uint8_t((shortFromBuffer >> bitPosInCurByte) & mask);
			return value;
		}

		template<unsigned NbBits, typename std::enable_if<(NbBits > 0)>::type* = nullptr>
		int32_t readSigned()
		{
			return signExtend<int32_t, NbBits>(readUnsigned(NbBits));
		}

		template<unsigned NbBits, typename std::enable_if<(NbBits == 0)>::type* = nullptr>
		int32_t readSigned()
		{
			currentBitPosition += NbBits;
			return 0;
		}
	};

	struct DCCHeader
	{
		uint8_t signature;		// Magic number for DCC files, must be 0x74
		uint8_t version;		// DCC major version, usually 6
		uint8_t directions;		// Number of directions in this file, max 32
		uint32_t framesPerDir;	// Frames number for each direction
		uint32_t tag;			// Seems to be always 1 ?

		// Size of the decoded image in bytes
		// (outsizeCoded for all directions) + directions*framesPerDir*4 + 24
		uint32_t finalDc6Size;
	};

	struct DCCDirectionHeader
	{
		uint32_t outsizeCoded;
		bool hasRawPixelEncoding : 1;	// Do we have a direction supporting the raw pixel encoding ?
		bool compressEqualCells : 1;	// Do we have a stream for the equal cells optimization ?
		uint32_t variable0Bits : 4;		// Endcoded size in bits of DCCFrameHeader::variable0
		uint32_t widthBits : 4;			// Endcoded size in bits of DCCFrameHeader::width
		uint32_t heightBits : 4;		// Endcoded size in bits of DCCFrameHeader::height
		uint32_t xOffsetBits : 4;		// Endcoded size in bits of DCCFrameHeader::xoffset
		uint32_t yOffsetBits : 4;		// Endcoded size in bits of DCCFrameHeader::yoffset
		uint32_t optionalBytesBits : 4;	// Endcoded size in bits of DCCFrameHeader::optionalBytes
		uint32_t codedBytesBits : 4;	// Endcoded size in bits of DCCFrameHeader::codedBytes
	};

	struct DCCFrameHeader
	{
		uint32_t variable0;
		uint32_t width;
		uint32_t height;
		int32_t xOffset;
		int32_t yOffset;
		uint32_t optionalBytes;
		uint32_t codedBytes;
		bool frameBottomUp;

		// Extent of this frame image in the pixel buffer.
		// Boundaries are included
		AABB<int32_t> extents;
	};

	struct DCCDirection
	{
		DCCDirectionHeader header;
		std::vector<DCCFrameHeader> frameHeaders;

		AABB<int32_t> extents;

		void computeDirExtents()
		{
			extents.initializeForExtension();
			for (const DCCFrameHeader& frame : frameHeaders)
			{
				extents.extend(frame.extents);
			}
		}
	};

	static constexpr unsigned bitsWidthTable[16] = {
		0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 26, 28, 30, 32
	};

	using readSignedPtrType = int32_t(BitStreamView::*)(void);

	static constexpr readSignedPtrType readSignedPtrs[16] = {
		&BitStreamView::readSigned<bitsWidthTable[0]>,
		&BitStreamView::readSigned<bitsWidthTable[1]>,
		&BitStreamView::readSigned<bitsWidthTable[2]>,
		&BitStreamView::readSigned<bitsWidthTable[3]>,
		&BitStreamView::readSigned<bitsWidthTable[4]>,
		&BitStreamView::readSigned<bitsWidthTable[5]>,
		&BitStreamView::readSigned<bitsWidthTable[6]>,
		&BitStreamView::readSigned<bitsWidthTable[7]>,
		&BitStreamView::readSigned<bitsWidthTable[8]>,
		&BitStreamView::readSigned<bitsWidthTable[9]>,
		&BitStreamView::readSigned<bitsWidthTable[10]>,
		&BitStreamView::readSigned<bitsWidthTable[11]>,
		&BitStreamView::readSigned<bitsWidthTable[12]>,
		&BitStreamView::readSigned<bitsWidthTable[13]>,
		&BitStreamView::readSigned<bitsWidthTable[14]>,
		&BitStreamView::readSigned<bitsWidthTable[15]> };

	static bool readDirHeader(DCCDirectionHeader& dirHeader, BitStreamView& bitStream)
	{
		dirHeader.outsizeCoded = bitStream.readUnsigned(32);
		dirHeader.hasRawPixelEncoding = bitStream.readBool();
		dirHeader.compressEqualCells = bitStream.readBool();
		dirHeader.variable0Bits = bitStream.readUnsigned8OrLess(4);
		dirHeader.widthBits = bitStream.readUnsigned8OrLess(4);
		dirHeader.heightBits = bitStream.readUnsigned8OrLess(4);
		dirHeader.xOffsetBits = bitStream.readUnsigned8OrLess(4);
		dirHeader.yOffsetBits = bitStream.readUnsigned8OrLess(4);
		dirHeader.optionalBytesBits = bitStream.readUnsigned8OrLess(4);
		dirHeader.codedBytesBits = bitStream.readUnsigned8OrLess(4);
		return bitStream.good();
	}

	static bool readFrameHeaders(uint32_t nbFrames, DCCDirection& outDir, BitStreamView& bitStream)
	{
		const DCCDirectionHeader& dirHeader = outDir.header;
		// Read all frame headers
		outDir.frameHeaders.resize(nbFrames);
		for (DCCFrameHeader& fHdr : outDir.frameHeaders)
		{
			// We are using member function pointers here because we would have one indirection
			// From looking up the size anyway, so we might as well call the template instance directly
			fHdr.variable0 = bitStream.readUnsigned(bitsWidthTable[dirHeader.variable0Bits]);
			fHdr.width = bitStream.readUnsigned(bitsWidthTable[dirHeader.widthBits]);
			fHdr.height = bitStream.readUnsigned(bitsWidthTable[dirHeader.heightBits]);
			fHdr.xOffset = (bitStream.*readSignedPtrs[dirHeader.xOffsetBits])();
			fHdr.yOffset = (bitStream.*readSignedPtrs[dirHeader.yOffsetBits])();

			fHdr.optionalBytes = bitStream.readUnsigned(bitsWidthTable[dirHeader.optionalBytesBits]);
			fHdr.codedBytes = bitStream.readUnsigned(bitsWidthTable[dirHeader.codedBytesBits]);
			fHdr.frameBottomUp = bitStream.readBool();

			assert(fHdr.width < 0x700000);
			assert(fHdr.height < 0x700000);
			fHdr.extents.xLower = fHdr.xOffset;
			fHdr.extents.xUpper = fHdr.xOffset + int32_t(fHdr.width);

			if (fHdr.frameBottomUp) {
				assert(false && "Please report the name of the DCC file to the devs!");
				fHdr.extents.yLower = fHdr.yOffset;
				// Upper excluded (max==upper-1)
				fHdr.extents.yUpper = fHdr.yOffset + int32_t(fHdr.height);
			}
			else // top-down
			{
				fHdr.extents.yLower = fHdr.yOffset - int32_t(fHdr.height) + 1;
				// Upper excluded (max==upper-1)
				fHdr.extents.yUpper = fHdr.yOffset + 1;
			}
		}

		// Handle optional data
		for (DCCFrameHeader& frameHeader : outDir.frameHeaders)
		{
			assert(!frameHeader.optionalBytes);
		}
		return bitStream.good();
	}

	// Do not expose internals
	// For the pixel buffer the maximum size of a cell is 4
	constexpr size_t pbCellMaxPixelSize = 4u;

	struct Cell
	{
		size_t width : 4;	// 3 bits would be engouh (max value is 5)
		size_t height : 4;	// 3 bits would be engouh (max value is 5)
	};

	// Each pixel buffer entry contains 4 pixels codes
	struct PixelBufferEntry
	{
		static constexpr size_t nbValues = 4;
		uint8_t values[nbValues];
	};

	struct DCCFrameData
	{
		size_t firstPixelBufferEntry;
		uint16_t nbCellsX;
		uint16_t nbCellsY;
		uint16_t offsetX;	// X Offset relative to the whole direction bounding box
		uint16_t offsetY;	// Y Offset relative to the whole direction bounding box

		std::vector<bool> cellSameAsPrevious;
		std::vector<uint8_t> cellWidths;
		std::vector<uint8_t> cellHeights;

		ImageView imageView;	// Output buffer image view

		DCCFrameData(const DCCDirection& dir, const DCCFrameHeader& frameHeader,
			SimpleImageProvider& imgProvider)
			: firstPixelBufferEntry(0)
		{
			offsetX = uint16_t(frameHeader.extents.xLower - dir.extents.xLower);
			offsetY = uint16_t(frameHeader.extents.yLower - dir.extents.yLower);

			// width (in # of pixels) in 1st column
			const uint16_t widthFirstColumn = 4 - (offsetX % 4);
			const uint16_t frameWidth = uint16_t(frameHeader.extents.width());
			if ((frameWidth - widthFirstColumn) <= 1)
				nbCellsX = 1; // if 2nd column is 0 or 1 pixel wide, only use 1 cell
			else
			{
				// so, we have minimum 2 pixels behind 1st column
				uint16_t tmp = frameWidth - widthFirstColumn - 1; // tmp is minimum 1, can't be 0
				nbCellsX = 2 + (tmp / 4);
				if ((tmp % 4) == 0) nbCellsX--;
			}

			const uint16_t heightFirstRow = 4 - (offsetY % 4);
			const uint16_t frameHeight = uint16_t(frameHeader.extents.height());
			if ((frameHeight - heightFirstRow) <= 1)
				nbCellsY = 1; // if 2nd row is 0 or 1 pixel high, only use 1 cell
			else
			{
				uint16_t tmp = frameHeight - heightFirstRow - 1;
				nbCellsY = 2 + (tmp / 4);
				if ((tmp % 4) == 0) nbCellsY--;
			}

			cellSameAsPrevious.resize(size_t(nbCellsX) * size_t(nbCellsY));

			// Initialize to 4 by default
			cellWidths.resize(nbCellsX, 4);
			cellHeights.resize(nbCellsY, 4);

			if (nbCellsX == 1)
				cellWidths[0] = uint8_t(frameWidth); // Might have merged 2nd column into 1st
			else
			{
				// Treat the special cases (first and last columns/rows)
				cellWidths[0] = uint8_t(widthFirstColumn);
				// Compute size of the last column
				const size_t nbColumnsExcludingFirstAndLast = nbCellsX - 2;
				const size_t widthExcludingFirstAndLastColumns = 4 * nbColumnsExcludingFirstAndLast;
				cellWidths[nbCellsX - 1] =
					uint8_t(frameWidth - (widthFirstColumn + widthExcludingFirstAndLastColumns));
			}

			if (nbCellsY == 1)
				cellHeights[0] = uint8_t(frameHeight); // Might have merged 2nd row into 1st
			else
			{
				cellHeights[0] = uint8_t(heightFirstRow);
				// Compute size of the last row
				const size_t nbRowsExcludingFirstAndLast = nbCellsY - 2;
				const size_t heightExcludingFirstAndLastRows = 4 * nbRowsExcludingFirstAndLast;
				cellHeights[nbCellsY - 1] =
					uint8_t(frameHeight - (heightFirstRow + heightExcludingFirstAndLastRows));
			}

			imageView = imgProvider.getNewImage(frameWidth, frameHeight);
		}
	};

	struct DCCDirectionData
	{
		const DCCDirection& dirRef;

		std::vector<uint8_t> codeToPixelValue;

		BitStreamView equalCellBitStream;
		BitStreamView pixelMaskBitStream;
		BitStreamView rawPixelUsageBitStream;
		BitStreamView rawPixelCodesBitStream;
		BitStreamView pixelCodesDisplacementBitStream;

		size_t nbFrames;
		size_t nbPixelBufferCellsX;
		size_t nbPixelBufferCellsY;

		std::vector<DCCFrameData> framesData;

		DCCDirectionData(const DCCDirection& dir, BitStreamView& bitStream, size_t nbFramesPerDir,
			SimpleImageProvider& imgProvider)
			: dirRef(dir), nbFrames(nbFramesPerDir)
		{
			uint32_t equalCellsBitStreamSize = 0;
			uint32_t pixelMaskBitStreamSize = 0;
			uint32_t encodingTypeBitsreamSize = 0;
			uint32_t rawPixelCodesBitStreamSize = 0;

			if (dir.header.compressEqualCells) {
				equalCellsBitStreamSize = bitStream.readUnsigned(20);
			}

			pixelMaskBitStreamSize = bitStream.readUnsigned(20);

			if (dir.header.hasRawPixelEncoding) {
				encodingTypeBitsreamSize = bitStream.readUnsigned(20);
				rawPixelCodesBitStreamSize = bitStream.readUnsigned(20);
			}

			// Tells what code correspond to which pixel value.
			// For example if the pixel values used are 0, 31 , 42 then
			// code 0 gives 0
			// code 1 gives 31
			// code 2 gives 42
			codeToPixelValue.reserve(256);
			for (size_t i = 0; i < 256; i++)
			{
				const bool pixelValueUsed = bitStream.readBool();
				if (pixelValueUsed) codeToPixelValue.push_back(uint8_t(i));
			}

			// Prepare the bitstreams

			assert(!dir.header.compressEqualCells || equalCellsBitStreamSize);
			equalCellBitStream = bitStream.createSubView(equalCellsBitStreamSize);
			bitStream.skip(equalCellsBitStreamSize);

			pixelMaskBitStream = bitStream.createSubView(pixelMaskBitStreamSize);
			bitStream.skip(pixelMaskBitStreamSize);

			assert(!dir.header.hasRawPixelEncoding ||
				(encodingTypeBitsreamSize && rawPixelCodesBitStreamSize));
			rawPixelUsageBitStream = bitStream.createSubView(encodingTypeBitsreamSize);
			bitStream.skip(encodingTypeBitsreamSize);

			rawPixelCodesBitStream = bitStream.createSubView(rawPixelCodesBitStreamSize);
			bitStream.skip(rawPixelCodesBitStreamSize);

			// Note : goes until the end of the direction
			pixelCodesDisplacementBitStream =
				bitStream.createSubView(bitStream.bufferSizeInBits() - bitStream.tell());

			const size_t dirWidth = size_t(dir.extents.width());
			const size_t dirHeight = size_t(dir.extents.height());

			// Compute the size in cells of the pixel buffer. There are no alignment nor dimensions
			// requirements for the pixel buffer, but cells are of size 4 at max.

			// nbPixelBufferCellsX = dirWidth/4 rounded up
			nbPixelBufferCellsX = 1u + (dirWidth - 1u) / pbCellMaxPixelSize;
			nbPixelBufferCellsY = 1u + (dirHeight - 1u) / pbCellMaxPixelSize;

			framesData.reserve(nbFrames);

			for (size_t frameIndex = 0; frameIndex < nbFrames; ++frameIndex)
			{
				framesData.emplace_back(dir, dir.frameHeaders[frameIndex], imgProvider);
			}
		}

		// Check that all the data is valid and allocated
		bool isValid() const
		{
			for (size_t frameIndex = 0; frameIndex < nbFrames; ++frameIndex)
			{
				if (!framesData[frameIndex].imageView.isValid()) return false;
			}
			return true;
		}
	};

	// Counts the number of bits set to 1.
	// Aka population count / Hamming weight.
	static uint16_t popCount(uint16_t value)
	{
		std::bitset<16> bset = value;
		return uint16_t(bset.count());
	}

	using PixelCodesStack = std::array<uint8_t, PixelBufferEntry::nbValues>;

	// returns the number of pixels codes decoded from the stream
	int decodePixelCodesStack(DCCDirectionData& data, uint8_t pixelMask, PixelCodesStack& pixelCodesStack)
	{
		if (!pixelMask) return 0; // Reuse the previous cell values, but still decode the cell in stage2
		const uint16_t nbPixelsInMask = popCount(uint16_t(pixelMask));

		// Is the cell encoded in the raw stream ?
		const bool decodeRaw = data.rawPixelUsageBitStream.bufferSizeInBits() > 0 &&
			data.rawPixelUsageBitStream.readBool();

		uint8_t lastPixelCode = 0;
		size_t curPixelIdx = 0;

		for (curPixelIdx = 0; curPixelIdx < nbPixelsInMask; curPixelIdx++)
		{
			uint8_t& curPixelCode = pixelCodesStack[curPixelIdx];
			if (decodeRaw) {
				// Read the value of the code directly from rawPixelCodesBitStream
				curPixelCode = data.rawPixelCodesBitStream.readUnsigned8OrLess(8);
			}
			else
			{
				// Read the value of the code incrementally from pixelCodesDisplacementBitStream
				curPixelCode = lastPixelCode;
				uint8_t pixelDisplacement;
				do
				{
					pixelDisplacement = data.pixelCodesDisplacementBitStream.readUnsigned8OrLess(4);
					curPixelCode += pixelDisplacement;
				} while (pixelDisplacement == 0xF);
			}
			// Stop decoding if we encounter twice the same pixel code.
			// It also means that this pixel code is discarded.
			if (curPixelCode == lastPixelCode) {
				// Note : We discard the pixel by putting a 0 but it doesn't matter anyway since we only
				// use nbPixelsDecoded values later when popping the stack
				curPixelCode = 0;
				break;
			}
			else
			{
				lastPixelCode = curPixelCode;
			}
		}
		return int(curPixelIdx);
	}

	void decodeFrameStage1(DCCDirectionData& data, DCCFrameData& frameData,
		std::vector<size_t>& pixelBuffer, std::vector<PixelBufferEntry>& pbEntries)
	{
		// Offset in terms of cells for this frame
		const size_t frameCellOffsetX = frameData.offsetX / 4;
		const size_t frameCellOffsetY = frameData.offsetY / 4;

		// For each cell of this frame (not the same number as the pixel buffer cells ! )
		for (size_t y = 0; y < frameData.nbCellsY; y++)
		{
			const size_t curCellY = frameCellOffsetY + y;
			for (size_t x = 0; x < frameData.nbCellsX; x++)
			{
				const size_t curCellX = frameCellOffsetX + x;

				const size_t curPbCellIndex = curCellX + curCellY * data.nbPixelBufferCellsX;
				const size_t curFrameCellIndex = x + y * frameData.nbCellsX;

				size_t& lastPixelEntryIndexForCell = pixelBuffer[curPbCellIndex];

				bool sameAsPreviousCell = false;	// By default always decode the cell
				uint8_t pixelMask = 0x0F;			// Default pixel mask

				// Check if this cell is equal to the previous one
				if (lastPixelEntryIndexForCell < pbEntries.size()) {
					// Check if we have to reuse the previous values
					if (data.dirRef.header.compressEqualCells) {
						// If true, the cell is the same as the previous one or transparent.
						// Which actually mean the same thing : skip the decoding of this cell
						sameAsPreviousCell = data.equalCellBitStream.readBool();
					}
					if (!sameAsPreviousCell) {
						pixelMask = data.pixelMaskBitStream.readUnsigned8OrLess(4);
					}
				}

				// Store the fact that we skipped this cell for the 2nd phase
				frameData.cellSameAsPrevious[curFrameCellIndex] = sameAsPreviousCell;

				if (!sameAsPreviousCell) {
					// Pixel buffer entries are encoded as a stack in the stream which means the
					// last value decoded is actually the 1st value with a bit in the mask.
					PixelCodesStack pixelCodesStack = {};
					int nbPixelsDecoded = decodePixelCodesStack(data, pixelMask, pixelCodesStack);

					PixelBufferEntry previousEntryForCell;
					if (lastPixelEntryIndexForCell < pbEntries.size()) {
						previousEntryForCell = pbEntries[lastPixelEntryIndexForCell];
					}
					else
					{
						// No need for previousEntryForCell if it doesn't exist, as the mask is 0xF
						assert(pixelMask == 0xF);
					}

					// Finalize the decoding of the pixel buffer entry
					PixelBufferEntry newEntry;

					int curIndex = nbPixelsDecoded - 1;
					for (size_t i = 0; i < PixelBufferEntry::nbValues; i++)
					{
						// Pop a value if bit set in the mask
						if (pixelMask & (1u << i)) {
							uint8_t pixelCode;
							if (curIndex >= 0) {
								pixelCode = pixelCodesStack[size_t(curIndex--)];
							}
							else
							{
								pixelCode = 0;
							}
							// Store the actual values instead of the codes
							newEntry.values[i] = data.codeToPixelValue[pixelCode];
						}
						// If not set, use the previous value for this entry
						else
						{
							newEntry.values[i] = previousEntryForCell.values[i];
						}
					}
					// Update the pixel buffer cell information
					lastPixelEntryIndexForCell = pbEntries.size();
					// Add the new entry for use in the 2nd stage
					pbEntries.push_back(newEntry);
				}
			}
		}
	}

	void decodeDirectionStage1(DCCDirectionData& data, std::vector<PixelBufferEntry>& pbEntries)
	{
		// For each cell store a PixelBufferEntry index that points to the last entry for this cell
		// This will be used to retrieve values from the previous frame
		constexpr size_t invalidIndex = std::numeric_limits<size_t>::max();
		const size_t pixelBufferNbCells = data.nbPixelBufferCellsX * data.nbPixelBufferCellsY;
		std::vector<size_t> pixelBuffer(pixelBufferNbCells, invalidIndex);

		// 1st phase of decoding : fill the pixel buffer
		// We actually fill a buffer of entries as to avoid storing empty entries
		for (size_t frameIndex = 0; frameIndex < data.nbFrames; ++frameIndex)
		{
			DCCFrameData& frameData = data.framesData[frameIndex];
			frameData.firstPixelBufferEntry = pbEntries.size();
			decodeFrameStage1(data, frameData, pixelBuffer, pbEntries);
		}
	}

	void decodeDirectionStage2(DCCDirectionData& data, const std::vector<PixelBufferEntry>& pbEntries)
	{
		// This is the reason why we need to stages, we don't have the offset of this bitstream
		BitStreamView& pixelCodeIndices = data.pixelCodesDisplacementBitStream;

		const size_t pbWidth = size_t(data.dirRef.extents.width());
		const size_t pbHeight = size_t(data.dirRef.extents.height());
		const size_t pbStride = pbWidth;
		const size_t nbPixelBufferCells = data.nbPixelBufferCellsX * data.nbPixelBufferCellsY;

		std::vector<Cell> pixelBufferCells(nbPixelBufferCells, Cell{ 0xF, 0xF });
		std::vector<uint8_t> pixelBufferColors(pbStride * pbHeight);
		ImageView pBuffer{ pixelBufferColors.data(), pbWidth, pbHeight, pbStride };

		// 2nd phase of decoding : Finish using the pixel buffer entries
		for (size_t frameIndex = 0; frameIndex < data.nbFrames; ++frameIndex)
		{
			const DCCFrameData& frameData = data.framesData[frameIndex];
			size_t pbEntryIndex = frameData.firstPixelBufferEntry;

			size_t pbCellPosY = frameData.offsetY;
			for (size_t cellY = 0; cellY < frameData.nbCellsY; cellY++)
			{
				size_t pbCellPosX = frameData.offsetX;
				for (size_t cellX = 0; cellX < frameData.nbCellsX; cellX++)
				{
					const size_t frameCellIndex = cellX + cellY * frameData.nbCellsX;
					Cell frameCell;
					frameCell.width = frameData.cellWidths[cellX];
					frameCell.height = frameData.cellHeights[cellY];

					const size_t pbCellIndex =
						(pbCellPosX / pbCellMaxPixelSize) +
						(pbCellPosY / pbCellMaxPixelSize) * data.nbPixelBufferCellsX;
					Cell& pbCell = pixelBufferCells[pbCellIndex];

					if (frameData.cellSameAsPrevious[frameCellIndex]) {
						if ((frameCell.width != pbCell.width) || (frameCell.height != pbCell.height)) {
							// Clear the cell to 0
							// If we used the previous frame pixels instead of reusing the same
							// buffer for all frames we would not need to clear this (it would be
							// initialized to 0 before writing any value to the frame pixels) But we
							// would then need to copy values if the size matched
							pBuffer.fillBytes(pbCellPosX, pbCellPosY, frameCell.width, frameCell.height,
								0);
						}
						else
						{
							// Same size, copy from previous cell in the buffer
							// Nothing to change !
						}
					}
					else
					{
						const auto& pixelValues = pbEntries[pbEntryIndex++].values;

						if (pixelValues[0] == pixelValues[1]) {
							// This means we only got one pixel code, so fill the cell with it
							pBuffer.fillBytes(pbCellPosX, pbCellPosY, frameCell.width, frameCell.height,
								pixelValues[0]);
						}
						else
						{
							int nbBitsToRead = 0;
							if (pixelValues[1] == pixelValues[2]) {
								// Stopped decoding after the 2nd value, only pixelValues[0] and
								// pixelValues[1] are different, so only 1bit needs to be read to choose
								// from those values
								nbBitsToRead = 1;
							}
							else // We need 2 bits to index 3-4 values
							{
								nbBitsToRead = 2;
							}

							// fill FRAME cell with pixels
							for (size_t y = 0; y < frameCell.height; y++)
							{
								for (size_t x = 0; x < frameCell.width; x++)
								{
									const uint8_t pixelCodeIndex =
										pixelCodeIndices.readUnsigned8OrLess(nbBitsToRead);
									// Note: This actually means that a cell (4x4 block) can use at most
									// 4 colors, a bit like DXT !
									const uint8_t pixelValue = pixelValues[pixelCodeIndex];

									pBuffer(pbCellPosX + x, pbCellPosY + y) = pixelValue;
								}
							}
						}
					}

					pbCell = frameCell;
					pbCellPosX += frameCell.width;
				}
				pbCellPosY += frameData.cellHeights[cellY];
			}
			// Done decoding this frame, now copy its content.
			const ImageView frameImageView = frameData.imageView;
			const ImageView pbFrameView = pBuffer.subView(
				frameData.offsetX, frameData.offsetY, frameImageView.width, frameImageView.height);
			assert(frameImageView.isValid() && pbFrameView.isValid());

			pbFrameView.copyTo(frameImageView);
		}
	}

	bool readDirection(const std::vector<uint8_t>& fileData,
		const std::vector<uint32_t>& directionsOffsets,
		uint32_t directions, uint32_t framesPerDir,
		DCCDirection& outDir, uint32_t dirIndex, SimpleImageProvider& imgProvider)
	{
		if (dirIndex >= directions)
		{
			return false;
		}

		const size_t directionEncodedSize = directionsOffsets[dirIndex + 1] - directionsOffsets[dirIndex];

		BitStreamView bitStream(
			fileData.data() + directionsOffsets[dirIndex], directionEncodedSize * 8
		);

		DCCDirectionHeader& dirHeader = outDir.header;
		if (!readDirHeader(dirHeader, bitStream)) return false;

		if (!readFrameHeaders(framesPerDir, outDir, bitStream)) return false;

		outDir.computeDirExtents();

		DCCDirectionData data{ outDir, bitStream, framesPerDir, imgProvider };
		if (!data.isValid()) return false;

		std::vector<PixelBufferEntry> pbEntries;
		{
			size_t estimatedNbEntries =
				(framesPerDir * data.nbPixelBufferCellsX * data.nbPixelBufferCellsY) / 4;
			pbEntries.reserve(estimatedNbEntries);
		}

		decodeDirectionStage1(data, pbEntries);

		decodeDirectionStage2(data, pbEntries);

		// Make sure we fully read the streams
		assert(data.equalCellBitStream.tell() == data.equalCellBitStream.sizeInBits());
		assert(data.pixelMaskBitStream.tell() == data.pixelMaskBitStream.sizeInBits());
		assert(data.rawPixelUsageBitStream.tell() == data.rawPixelUsageBitStream.sizeInBits());
		assert(data.rawPixelCodesBitStream.tell() == data.rawPixelCodesBitStream.sizeInBits());
		// This exact stream size is not known, so check if we at least are in the last byte
		assert(data.pixelCodesDisplacementBitStream.bitPositionInBuffer() + (size_t)7 >=
			data.pixelCodesDisplacementBitStream.bufferSizeInBits());

		return bitStream.good();
	}
}

DCCImageContainer::DCCImageContainer(const std::shared_ptr<FileBytes>& fileBytes) : fileData(fileBytes)
{
	LittleEndianStreamReader fileStream(fileData->data(), fileData->size());

	// DCC header decode

	DCCHeader header;
	fileStream.read(header.signature);
	fileStream.read(header.version);
	fileStream.read(header.directions);
	fileStream.read(header.framesPerDir);
	fileStream.read(header.tag);
	fileStream.read(header.finalDc6Size);

	if (header.signature != 0x74 ||
		header.directions > 32)
	{
		return;
	}

	directions = header.directions;
	framesPerDir = header.framesPerDir;
	numberOfFrames = directions * framesPerDir;

	directionsOffsets.resize(header.directions + 1);
	directionsOffsets[header.directions] = uint32_t(fileStream.size());

	for (uint32_t dir = 0; dir < header.directions; dir++)
	{
		fileStream.read(directionsOffsets[dir]);
	}
}

sf::Image2 DCCImageContainer::get(uint32_t index,
	const PaletteArray* palette, ImageInfo& imgInfo) const
{
	if (index >= size())
	{
		return {};
	}

	auto directionIdx = index / framesPerDir;
	auto frameIdx = index % framesPerDir;

	DCCDirection d;
	SimpleImageProvider imgProvider;
	if (readDirection(*fileData, directionsOffsets, directions, framesPerDir,
		d, directionIdx, imgProvider) == true)
	{
		if (frameIdx < imgProvider.getImagesNumber())
		{
			auto imgView = imgProvider.getImage(frameIdx);
			sf::Image2 img;
			img.create((unsigned)imgView.width, (unsigned)imgView.height);
			for (size_t j = 0; j < imgView.height; j++)
			{
				for (size_t i = 0; i < imgView.width; i++)
				{
					img.setPixel((unsigned)i, (unsigned)j, ImageContainer::getColor(imgView(i, j), palette));
				}
			}

			imgInfo.offset.x = (float)d.frameHeaders[frameIdx].xOffset;
			imgInfo.offset.y = (float)d.frameHeaders[frameIdx].yOffset;
			if (d.frameHeaders[frameIdx].frameBottomUp == false)
			{
				imgInfo.offset.y -= (float)d.frameHeaders[frameIdx].height;
			}
			imgInfo.absoluteOffset = true;
			imgInfo.blendMode = blendMode;
			imgInfo.nextIndex = -1;

			return img;
		}
	}
	return {};
}
