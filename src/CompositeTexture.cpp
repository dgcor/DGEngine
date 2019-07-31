#include "CompositeTexture.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include <algorithm>
#include "gsl/gsl"
#include "PhysFSStream.h"
#include "StreamReader.h"
#endif
#include "TextureInfo.h"

#ifndef NO_DIABLO_FORMAT_SUPPORT
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

void CompositeTexture::addGroup(const std::string_view fileName)
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

	if (addGroup(header.layers) == false)
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
	size_t layersOrdersSize = header.frames * header.directions * header.layers;
	auto layersOrdersStartIdx = layersOrders.size();
	layersOrders.resize(layersOrdersStartIdx + layersOrdersSize);
	fileStream.read((uint8_t*)layersOrders.data() + layersOrdersStartIdx, layersOrdersSize);

	// normalize indexes from 0-16 to 0-<number of layers>
	uint8_t newIdx = 0;
	for (auto cofIdx : layerIndexes)
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

	compositeTextureGroups.back().hasAllLayersOrdersDirections = true;
}
#endif

bool CompositeTexture::addGroup(uint32_t texturePackCount)
{
	if (compositeTextureGroups.empty() == true)
	{
		if (texturePackCount > 0 && texturePackCount <= texturePacks.size())
		{
			CompositeTextureGroup group;
			group.texturePackStartIdx = 0;
			group.texturePackStopIdx = texturePackCount - 1;
			group.texturePackGroups = texturePacks.front()->getGroupCount();
			if (group.texturePackGroups > 1)
			{
				hasMultipleGroupsPerTexturePack = true;
			}
			totalTexturePackGroups += group.texturePackGroups;
			group.directions = texturePacks.front()->getDirectionCount(0);
			group.range = texturePacks.front()->getRange(-1, -1);
			compositeTextureGroups.push_back(group);
			numberOfFrames += texturePacks.front()->size();
			return true;
		}
	}
	else
	{
		if (texturePackCount > 0)
		{
			auto texturePackStartIdx = compositeTextureGroups.back().texturePackStopIdx + 1;
			auto texturePackStopIdx = texturePackStartIdx + texturePackCount - 1;
			if (texturePackStopIdx >= texturePackStartIdx &&
				texturePackStopIdx < texturePacks.size())
			{
				CompositeTextureGroup group;
				group.texturePackStartIdx = texturePackStartIdx;
				group.texturePackStopIdx = texturePackStopIdx;
				group.texturePackGroups = texturePacks[texturePackStartIdx]->getGroupCount();
				if (group.texturePackGroups > 1)
				{
					hasMultipleGroupsPerTexturePack = true;
				}
				totalTexturePackGroups += group.texturePackGroups;
				group.directions = texturePacks.front()->getDirectionCount(0);
				group.rangeStartIdx = compositeTextureGroups.back().range.second + 1;
				group.range = texturePacks[texturePackStartIdx]->getRange(-1, -1);
				group.range.first += group.rangeStartIdx;
				group.range.second += group.rangeStartIdx;
				compositeTextureGroups.push_back(group);
				numberOfFrames += texturePacks[texturePackStartIdx]->size();
				return true;
			}
		}
	}
	return false;
}

void CompositeTexture::addTexturePack(const std::shared_ptr<TexturePack>& texture)
{
	texturePacks.push_back(std::move(texture));
}

void CompositeTexture::setLayersOrders(const std::vector<int8_t>& groupLayersOrders)
{
	if (compositeTextureGroups.empty() == true ||
		groupLayersOrders.empty() == true)
	{
		return;
	}

	auto layersOrdersStartIdx = layersOrders.empty() == true ? 0 : layersOrders.size() - 1;
	auto layersOrdersStopIdx = layersOrdersStartIdx + groupLayersOrders.size() - 1;

	layersOrders.insert(layersOrders.end(), groupLayersOrders.begin(), groupLayersOrders.end());

	compositeTextureGroups.back().layersOrdersStartIdx = layersOrdersStartIdx;
	compositeTextureGroups.back().layersOrdersStopIdx = layersOrdersStopIdx;

	compositeTextureGroups.back().hasAllLayersOrdersDirections = false;
}

uint32_t CompositeTexture::getTextureCount() const noexcept
{
	return numberOfFrames;
}

uint32_t CompositeTexture::getGroupCount() const noexcept
{
	return totalTexturePackGroups;
}

uint32_t CompositeTexture::getDirectionCount(uint32_t groupIdx) const
{
	if (hasMultipleGroupsPerTexturePack == false)
	{
		if (groupIdx < compositeTextureGroups.size())
		{
			return compositeTextureGroups[groupIdx].directions;
		}
	}
	else
	{
		uint32_t numGroups = 0;
		for (const auto& group : compositeTextureGroups)
		{
			numGroups += group.texturePackGroups;
			if (groupIdx < numGroups)
			{
				return group.directions;
			}
		}
	}
	return 1;
}

uint32_t CompositeTexture::getLayerCount(uint32_t groupIdx) const noexcept
{
	if (compositeTextureGroups.size() <= 1)
	{
		return texturePacks.size();
	}
	else if (hasMultipleGroupsPerTexturePack == false)
	{
		if (groupIdx < compositeTextureGroups.size())
		{
			const auto& group = compositeTextureGroups[groupIdx];
			return group.texturePackStopIdx - group.texturePackStartIdx + 1;
		}
	}
	else
	{
		uint32_t numGroups = 0;
		for (const auto& group : compositeTextureGroups)
		{
			numGroups += group.texturePackGroups;
			if (groupIdx < numGroups)
			{
				return group.texturePackStopIdx - group.texturePackStartIdx + 1;
			}
		}
	}
	return 0;
}

bool CompositeTexture::get(uint32_t index, std::vector<TextureInfo>& tiVec) const
{
	tiVec.clear();

	size_t texPackStartIdx = 0;
	size_t texPackStopIdx = 0;
	size_t numTexturePacks = 0;
	size_t loStartIdx = 0;
	size_t loStopIdx = 0;

	if (compositeTextureGroups.empty() == false)
	{
		for (const auto& group : compositeTextureGroups)
		{
			if (index >= group.range.first &&
				index <= group.range.second)
			{
				texPackStartIdx = group.texturePackStartIdx;
				texPackStopIdx = group.texturePackStopIdx;
				index -= group.range.first;

				if (layersOrders.empty() == true)
				{
					break;
				}

				numTexturePacks = texPackStopIdx - texPackStartIdx + 1;
				if (group.hasAllLayersOrdersDirections == false)
				{
					auto directionIdx = texturePacks[texPackStartIdx]->getDirection(index);
					if (directionIdx < group.directions)
					{
						loStartIdx = directionIdx * numTexturePacks;
						loStopIdx = std::min(loStartIdx + numTexturePacks, layersOrders.size());
					}
				}
				else
				{
					loStartIdx = group.layersOrdersStartIdx + (index * numTexturePacks);
					loStopIdx = std::min(loStartIdx + numTexturePacks, layersOrders.size());
				}
				break;
			}
		}
	}

	if (loStartIdx < loStopIdx)
	{
		for (size_t i = loStartIdx; i < loStopIdx; i++)
		{
			auto idx = layersOrders[i];
			if (idx >= 0 && (size_t)idx < numTexturePacks)
			{
				auto texPackIdx = texPackStartIdx + idx;
				if (texPackIdx < texturePacks.size())
				{
					TextureInfo ti;
					if (texturePacks[texPackIdx]->get(index, ti) == true)
					{
						tiVec.push_back(ti);
					}
				}
			}
		}
	}
	else
	{
		for (size_t i = texPackStartIdx; i <= texPackStopIdx; i++)
		{
			TextureInfo ti;
			if (texturePacks[i]->get(index, ti) == true)
			{
				tiVec.push_back(ti);
			}
		}
	}
	return tiVec.empty() == false;
}

std::pair<uint32_t, uint32_t> CompositeTexture::getRange(
	int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const
{
	if (compositeTextureGroups.empty() == true)
	{
		animType = {};
		return {};
	}
	else if (groupIdx >= 0)
	{
		if (compositeTextureGroups.size() == 1)
		{
			return texturePacks.front()->getRange(groupIdx, directionIdx, animType);
		}
		else if (hasMultipleGroupsPerTexturePack == false)
		{
			if ((size_t)groupIdx < compositeTextureGroups.size())
			{
				const auto& group = compositeTextureGroups[groupIdx];
				auto range = texturePacks[group.texturePackStartIdx]->getRange(
					0, directionIdx, animType
				);
				range.first += group.rangeStartIdx;
				range.second += group.rangeStartIdx;
				return range;
			}
		}
		else
		{
			int32_t normalizedGroupIdx = groupIdx;
			for (const auto& group : compositeTextureGroups)
			{
				if ((uint32_t)normalizedGroupIdx < group.texturePackGroups)
				{
					auto range = texturePacks[group.texturePackStartIdx]->getRange(
						normalizedGroupIdx, directionIdx, animType
					);
					range.first += group.rangeStartIdx;
					range.second += group.rangeStartIdx;
					return range;
				}
				else
				{
					normalizedGroupIdx -= (int32_t)group.texturePackGroups;
				}
			}
		}
	}
	animType = {};
	return std::make_pair(
		compositeTextureGroups.front().range.first,
		compositeTextureGroups.back().range.second
	);
}

std::pair<uint32_t, uint32_t> CompositeTexture::getRange(
	int32_t groupIdx, int32_t directionIdx) const
{
	AnimationType animType;
	return getRange(groupIdx, directionIdx, animType);
}
