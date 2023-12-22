#pragma once

#include "Game/AnimationInfo.h"

namespace TexturePackUtils
{
	// returns direction from frame index
	// range is exclusive [startIdx, numFrames)
	// sets AnimationFlags
	uint32_t calculateDirection(uint32_t frameIdx, uint32_t startIdx,
		uint32_t numFrames, uint32_t directions, AnimationFlags& flags);

	// sets AnimationInfo indexRange + AnimationFlags from direction
	// range is inclusive [startIdx, stopIdx]
	// ex: [0, 2] has 3 indexes -> 0, 1, 2
	// returns true if animation is valid
	bool calculateRange(uint32_t startIdx, uint32_t stopIdx,
		int32_t directionIdx, uint32_t directions, AnimationInfo& animInfo);
};
