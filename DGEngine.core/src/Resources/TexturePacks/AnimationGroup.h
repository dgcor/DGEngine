#pragma once

#include "Game/AnimationInfo.h"
#include <vector>

struct AnimationGroup
{
	std::pair<uint32_t, uint32_t> indexRange;
	uint32_t directions;
	std::vector<std::pair<uint32_t, uint32_t>> directionsVec;
	AnimationType animType;
	sf::Time refresh;

	uint32_t size() const noexcept { return indexRange.second + 1 - indexRange.first; }

	uint32_t getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const;
};
