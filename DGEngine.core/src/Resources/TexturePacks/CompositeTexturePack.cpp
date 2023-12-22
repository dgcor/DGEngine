#include "CompositeTexturePack.h"
#include "Game/AnimationInfo.h"

bool CompositeTexturePack::addGroup()
{
	if (compositeTextureGroups.empty() == true)
	{
		if (texturePacks.empty() == false)
		{
			CompositeTextureGroup group;
			group.texturePackStartIdx = 0;
			group.texturePackStopIdx = (uint32_t)texturePacks.size() - 1;
			group.texturePackGroups = texturePacks.front()->getGroupCount();
			totalTexturePackGroups += group.texturePackGroups;
			group.directions = texturePacks.front()->getDirectionCount(0);
			group.range = texturePacks.front()->getAnimation(-1, -1).indexRange;
			compositeTextureGroups.push_back(group);
			numberOfFrames += group.range.second + 1 - group.range.first;
			return true;
		}
	}
	else
	{
		auto texturePackStartIdx = compositeTextureGroups.back().texturePackStopIdx + 1;
		auto texturePackCount = (uint32_t)texturePacks.size() - texturePackStartIdx;
		auto texturePackStopIdx = texturePackStartIdx + texturePackCount - 1;
		if (texturePackStopIdx >= texturePackStartIdx &&
			texturePackStopIdx < texturePacks.size())
		{
			CompositeTextureGroup group;
			group.texturePackStartIdx = texturePackStartIdx;
			group.texturePackStopIdx = texturePackStopIdx;
			group.texturePackGroups = texturePacks[texturePackStartIdx]->getGroupCount();
			totalTexturePackGroups += group.texturePackGroups;
			group.directions = texturePacks.front()->getDirectionCount(0);
			group.rangeStartIdx = compositeTextureGroups.back().range.second + 1;
			group.range = texturePacks[texturePackStartIdx]->getAnimation(-1, -1).indexRange;
			group.range.first += group.rangeStartIdx;
			group.range.second += group.rangeStartIdx;
			compositeTextureGroups.push_back(group);
			numberOfFrames += group.range.second + 1 - group.range.first;
			return true;
		}
	}
	return false;
}

void CompositeTexturePack::setLayersOrders(const std::vector<int8_t>& groupLayersOrders)
{
	if (compositeTextureGroups.empty() == true ||
		groupLayersOrders.empty() == true)
	{
		return;
	}

	auto layersOrdersStartIdx = layersOrders.empty() == true ? 0 : (uint32_t)layersOrders.size();
	auto layersOrdersStopIdx = layersOrdersStartIdx + (uint32_t)groupLayersOrders.size() - 1;

	layersOrders.insert(layersOrders.end(), groupLayersOrders.begin(), groupLayersOrders.end());

	compositeTextureGroups.back().layersOrdersStartIdx = layersOrdersStartIdx;
	compositeTextureGroups.back().layersOrdersStopIdx = layersOrdersStopIdx;
}

uint32_t CompositeTexturePack::getLayerCount(uint32_t groupIdx) const noexcept
{
	if (compositeTextureGroups.size() <= 1)
	{
		return (uint32_t)texturePacks.size();
	}
	for (uint32_t numGroups = 0; const auto & group : compositeTextureGroups)
	{
		numGroups += group.texturePackGroups;
		if (groupIdx < numGroups)
		{
			return group.texturePackStopIdx - group.texturePackStartIdx + 1;
		}
	}
	return 0;
}

void CompositeTexturePack::processTexturePacksInOrder(uint32_t index, const processTexturePacksInOrderFunc processFunc) const
{
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
				index -= group.rangeStartIdx;

				if (layersOrders.empty() == true)
				{
					break;
				}

				numTexturePacks = texPackStopIdx - texPackStartIdx + 1;
				if (numberOfFrames * numTexturePacks != layersOrders.size())
				{
					// use same layer order for all texturePacks
					auto directionIdx = texturePacks[texPackStartIdx]->getDirection(index);
					if (directionIdx < group.directions)
					{
						loStartIdx = directionIdx * numTexturePacks;
						loStopIdx = std::min(loStartIdx + numTexturePacks, layersOrders.size());
					}
				}
				else
				{
					// use different layer order for each texturePack
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
					if (processFunc(index, texturePacks[texPackIdx]) == false)
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		for (size_t i = texPackStartIdx; i <= texPackStopIdx; i++)
		{
			if (processFunc(index, texturePacks[i]) == false)
			{
				break;
			}
		}
	}
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

	processTexturePacksInOrder(index,
		[&tiVec](uint32_t index, const std::shared_ptr<TexturePack>& texturePack) -> bool
		{
			TextureInfo ti;
			if (texturePack->get(index, ti) == true)
			{
				tiVec.push_back(ti);
			}
			return true;
		});

	return tiVec.empty() == false;
}

sf::Vector2i CompositeTexturePack::getTextureSize(uint32_t index) const
{
	sf::Vector2i textureSize;

	processTexturePacksInOrder(index,
		[&textureSize](uint32_t index, const std::shared_ptr<TexturePack>& texturePack) -> bool
		{
			textureSize = texturePack->getTextureSize(index);
			return textureSize.x == 0 || textureSize.y == 0;
		});

	return textureSize;
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
	for (uint32_t numGroups = 0; const auto& group : compositeTextureGroups)
	{
		numGroups += group.texturePackGroups;
		if (groupIdx < numGroups)
		{
			return group.directions;
		}
	}
	return 1;
}

AnimationInfo CompositeTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;

	if (compositeTextureGroups.empty() == true)
	{
		animInfo.flags = AnimationFlags::Overflow;
		return animInfo;
	}
	else if (groupIdx >= 0)
	{
		if (compositeTextureGroups.size() == 1)
		{
			return texturePacks.front()->getAnimation(groupIdx, directionIdx);
		}
		else
		{
			for (int32_t normalizedGroupIdx = groupIdx; const auto& group : compositeTextureGroups)
			{
				if ((uint32_t)normalizedGroupIdx < group.texturePackGroups)
				{
					animInfo = texturePacks[group.texturePackStartIdx]->getAnimation(
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
	animInfo.indexRange = {
		compositeTextureGroups.front().range.first,
		compositeTextureGroups.back().range.second
	};
	animInfo.flags = AnimationFlags::Overflow;
	return animInfo;
}
