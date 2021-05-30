#include "CompositeTexturePack.h"
#include "AnimationInfo.h"

bool CompositeTexturePack::addGroup(uint32_t texturePackCount)
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
			group.range = texturePacks.front()->getAnimation(-1, -1).indexRange;
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
				group.range = texturePacks[texturePackStartIdx]->getAnimation(-1, -1).indexRange;
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

void CompositeTexturePack::addTexturePack(const std::shared_ptr<TexturePack>& texture)
{
	texturePacks.push_back(texture);
}

void CompositeTexturePack::setLayersOrders(const std::vector<int8_t>& groupLayersOrders)
{
	if (compositeTextureGroups.empty() == true ||
		groupLayersOrders.empty() == true)
	{
		return;
	}

	auto layersOrdersStartIdx = layersOrders.empty() == true ? 0 : (uint32_t)layersOrders.size() - 1;
	auto layersOrdersStopIdx = layersOrdersStartIdx + (uint32_t)groupLayersOrders.size() - 1;

	layersOrders.insert(layersOrders.end(), groupLayersOrders.begin(), groupLayersOrders.end());

	compositeTextureGroups.back().layersOrdersStartIdx = layersOrdersStartIdx;
	compositeTextureGroups.back().layersOrdersStopIdx = layersOrdersStopIdx;

	compositeTextureGroups.back().hasAllLayersOrdersDirections = false;
}

uint32_t CompositeTexturePack::getLayerCount(uint32_t groupIdx) const noexcept
{
	if (compositeTextureGroups.size() <= 1)
	{
		return (uint32_t)texturePacks.size();
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

bool CompositeTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	std::vector<TextureInfo> tiVec;
	if (get(index, tiVec) == true)
	{
		ti = tiVec.front();
		return true;
	}
	return false;
}

bool CompositeTexturePack::get(uint32_t index, TextureInfoVar& tiVar) const
{
	std::vector<TextureInfo> tiVec;
	if (get(index, tiVec) == true)
	{
		tiVar.emplace<std::vector<TextureInfo>>(std::move(tiVec));
		return true;
	}
	return false;
}

bool CompositeTexturePack::get(uint32_t index, std::vector<TextureInfo>& tiVec) const
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

int32_t CompositeTexturePack::getWidth(uint32_t index) const
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getWidth(index);
	}
	return 0;
}

const std::shared_ptr<Palette>& CompositeTexturePack::getPalette() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getPalette();
	}
	return palette;
}

uint32_t CompositeTexturePack::size() const noexcept
{
	return numberOfFrames;
}

uint32_t CompositeTexturePack::getGroupCount() const noexcept
{
	return totalTexturePackGroups;
}

uint32_t CompositeTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
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

uint32_t CompositeTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	// not implemented
	return 0;
}

AnimationInfo CompositeTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (compositeTextureGroups.empty() == true)
	{
		return {};
	}
	else if (groupIdx >= 0)
	{
		if (compositeTextureGroups.size() == 1)
		{
			return texturePacks.front()->getAnimation(groupIdx, directionIdx);
		}
		else if (hasMultipleGroupsPerTexturePack == false)
		{
			if ((size_t)groupIdx < compositeTextureGroups.size())
			{
				const auto& group = compositeTextureGroups[groupIdx];
				auto animInfo = texturePacks[group.texturePackStartIdx]->getAnimation(
					0, directionIdx
				);
				animInfo.indexRange.first += group.rangeStartIdx;
				animInfo.indexRange.second += group.rangeStartIdx;
				return animInfo;
			}
		}
		else
		{
			int32_t normalizedGroupIdx = groupIdx;
			for (const auto& group : compositeTextureGroups)
			{
				if ((uint32_t)normalizedGroupIdx < group.texturePackGroups)
				{
					auto animInfo = texturePacks[group.texturePackStartIdx]->getAnimation(
						normalizedGroupIdx, directionIdx
					);
					animInfo.indexRange.first += group.rangeStartIdx;
					animInfo.indexRange.second += group.rangeStartIdx;
					return animInfo;
				}
				else
				{
					normalizedGroupIdx -= (int32_t)group.texturePackGroups;
				}
			}
		}
	}
	AnimationInfo animInfo;
	animInfo.indexRange = std::make_pair(
		compositeTextureGroups.front().range.first,
		compositeTextureGroups.back().range.second
	);
	return animInfo;
}
