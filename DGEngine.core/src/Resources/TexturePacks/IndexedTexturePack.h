#pragma once

#include "TexturePackIndexWrapper.h"
#include <unordered_map>
#include "Utils/PairXY.h"

class IndexedTexturePack : public TexturePackIndexWrapper
{
protected:
	std::unordered_map<uint32_t, uint32_t> textureIndexes;
	std::pair<uint32_t, uint32_t> indexRange;
	bool onlyUseIndexed{ false };

public:
	IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_, bool onlyUseIndexed_);

	// maps index to the current size of mapped indexes (10 -> 0, 20 -> 1, 21 -> 2)
	// if reverseMap is true, maps the current size of mapped indexes to index (0 -> 10, 1 -> 20, 2 -> 21)
	void mapTextureIndex(uint32_t mapIndex, bool reverseMap = false);

	// maps index to another index (10 -> 0, 20 -> 22)
	void mapTextureIndex(uint32_t mapIndex, uint32_t toIndex);

	// maps index range
	// if range stop index is smaller than start index, maps in reverse
	void mapTextureIndexRange(const PairInt32& indexRange,
		int32_t indexRangeStart = -1, bool reverseMap = false);

	bool translateIndex(uint32_t& index) const override;

	uint32_t size() const noexcept override;

	uint32_t getGroupCount() const noexcept override;

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
