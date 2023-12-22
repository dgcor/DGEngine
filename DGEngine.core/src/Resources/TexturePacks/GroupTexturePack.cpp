#include "GroupTexturePack.h"

void GroupTexturePack::addGroup(AnimationGroup&& group)
{
	if (group.directionsVec.empty() == true)
	{
		if (group.directions == 0)
		{
			group.directions = 1;
		}
	}
	else
	{
		group.directions = (uint32_t)group.directionsVec.size();
	}
	groups.push_back(std::move(group));
}

uint32_t GroupTexturePack::getGroupCount() const noexcept
{
	if (groups.size() > 0)
	{
		return (uint32_t)groups.size();
	}
	return texturePack->getGroupCount();
}

uint32_t GroupTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (groupIdx >= 0 && (size_t)groupIdx < groups.size())
	{
		return groups[groupIdx].directions;
	}
	return texturePack->getDirectionCount(groupIdx);
}

std::pair<uint32_t, uint32_t> GroupTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	if (groups.empty() == false)
	{
		for (uint32_t groupIdx = 0; const auto& group : groups)
		{
			auto direction = group.getDirection(frameIdx, flags);
			if ((int)(flags & AnimationFlags::Valid) != 0)
			{
				return { groupIdx, direction };
			}
			groupIdx++;
		}
		flags = AnimationFlags::Overflow;
		return {};
	}
	return texturePack->getDirection(frameIdx, flags);
}

AnimationInfo GroupTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (groupIdx >= 0 && (size_t)groupIdx < groups.size())
	{
		return groups[groupIdx].getAnimation(0, directionIdx);
	}
	return texturePack->getAnimation(groupIdx, directionIdx);
}
