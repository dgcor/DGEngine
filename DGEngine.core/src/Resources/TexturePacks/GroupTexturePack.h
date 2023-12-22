#pragma once

#include "AnimationGroup.h"
#include "TexturePackWrapper.h"
#include <vector>

class GroupTexturePack : public TexturePackWrapper
{
protected:
	std::vector<AnimationGroup> groups;

public:
	GroupTexturePack(std::unique_ptr<TexturePack> texturePack_)
		: TexturePackWrapper(std::move(texturePack_)) {}

	void addGroup(AnimationGroup&& group);

	uint32_t getGroupCount() const noexcept override;

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
