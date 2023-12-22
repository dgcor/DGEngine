#include "CompositeTexturePack2.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include <algorithm>
#include "SFML/PhysFSStream.h"
#include "Utils/StreamReader.h"
#endif

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
namespace
{
	struct COFHeader
	{
		uint8_t layers;
		uint8_t frames;
		uint8_t directions;
		uint8_t version;
		uint32_t unknown;
		int32_t xMin;
		int32_t xMax;
		int32_t yMin;
		int32_t yMax;
		int16_t animRate;	// Default animation rate(speed) in 8-bit fixed-point: 256 == 1.f.
		int16_t zero;
	};

	struct COFLayer
	{
		uint8_t index;
		uint8_t castsShadow;
		uint8_t selectable;
		uint8_t overrideTranslvl;
		uint8_t newTranslvl;
		char weaponClass[4];
	};

	// Frame trigger type
	enum class COFKeyframe : uint8_t
	{
		None,
		Attack,
		Missile,
		Sound,
		Skill,
		Max
	};

	// default order of layers
	// HD, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8
}

void CompositeTexturePack2::addGroup(const std::string_view fileName, bool fixLayerOrdering)
{
	std::vector<uint8_t> fileData;

	{
		sf::PhysFSStream file(fileName.data());
		if (file.hasError() == true)
		{
			return;
		}
		fileData.resize((size_t)file.getSize());
		file.read(fileData.data(), file.getSize());
	}

	if (fileData.size() < sizeof(COFHeader))
	{
		return;
	}

	LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

	COFHeader header;

	fileStream.read(header.layers);
	fileStream.read(header.frames);
	fileStream.read(header.directions);
	fileStream.read(header.version);
	fileStream.read(header.unknown);
	fileStream.read(header.xMin);
	fileStream.read(header.xMax);
	fileStream.read(header.yMin);
	fileStream.read(header.yMax);
	fileStream.read(header.animRate);
	fileStream.read(header.zero);

	if (header.version != 20 ||
		header.layers == 0 ||
		header.layers > 16 ||
		header.frames == 0)
	{
		return;
	}

	if (addGroup() == false)
	{
		return;
	}

	if (compositeTextureGroups.back().directions != header.directions)
	{
		return;
	}

	// layers
	std::vector<uint8_t> layerIndexes;
	layerIndexes.resize(header.layers);
	for (auto& idx : layerIndexes)
	{
		fileStream.peek(idx);
		fileStream.skip(sizeof(COFLayer));
	}
	std::sort(layerIndexes.begin(), layerIndexes.end());

	// keyframes
	fileStream.skip(header.frames);

	// layer order
	uint32_t layersOrdersSize = header.frames * header.directions * header.layers;
	uint32_t layersOrdersStartIdx = (uint32_t)layersOrders.size();
	layersOrders.resize((size_t)layersOrdersStartIdx + (size_t)layersOrdersSize);

	bool layerOrderingWasRead = false;

	if (fixLayerOrdering == true &&
		(header.directions == 8 || header.directions == 16 || header.directions == 32))
	{
		// Re-map COF directions
		layerOrderingWasRead = true;
		for (size_t cofDirection = 0; cofDirection < header.directions; cofDirection++)
		{
			auto newDirection = cofDirection;

			if (header.directions == 8)
			{
				static const uint8_t directionTable[8] = {
					4, 0, 5, 1, 6, 2, 7, 3
				};
				newDirection = directionTable[cofDirection];
			}
			else if (header.directions == 16)
			{
				static const uint8_t directionTable[16] = {
					4, 8, 0, 9, 5, 10, 1, 11, 6, 12, 2, 13, 7, 14, 3, 15
				};
				newDirection = directionTable[cofDirection];
			}
			else if (header.directions == 32)
			{
				static const uint8_t directionTable[32] = {
					4, 16, 8, 17, 0, 18, 9, 19, 5, 20, 10, 21, 1, 22, 11, 23,
					6, 24, 12, 25, 2, 26, 13, 27, 7, 28, 14, 29, 3, 30, 15, 31
				};
				newDirection = directionTable[cofDirection];
			}

			auto len = header.frames * header.layers;
			auto destIdx = layersOrdersStartIdx + newDirection * len;
			fileStream.read((uint8_t*)layersOrders.data() + destIdx, len);
		}
	}
	if (layerOrderingWasRead == false)
	{
		fileStream.read((uint8_t*)layersOrders.data() + layersOrdersStartIdx, layersOrdersSize);
	}

	// normalize indexes from 0-16 to 0-<number of layers>
	for (uint8_t newIdx = 0; auto cofIdx : layerIndexes)
	{
		for (size_t i = layersOrdersStartIdx; i < layersOrders.size(); i++)
		{
			if (layersOrders[i] == cofIdx)
			{
				layersOrders[i] = newIdx;
			}
		}
		newIdx++;
	}

	compositeTextureGroups.back().layersOrdersStartIdx = layersOrdersStartIdx;
	compositeTextureGroups.back().layersOrdersStopIdx = layersOrdersStartIdx + layersOrdersSize - 1;
}
#endif
