#pragma once

#include "Game/AnimationFlags.h"
#include "Game/AnimationType.h"
#include "Resources/TexturePack.h"
#include <vector>

struct TextureGroup
{
	std::shared_ptr<sf::Texture> texture;
	sf::Vector2f offset;
	uint32_t startIndex{ 0 };
	uint32_t maxFrames{ 0 };
	uint32_t numFrames{ 0 };
	uint32_t subImageSizeX{ 0 };
	uint32_t subImageSizeY{ 0 };
	bool verticalDirection{ false };

	bool makeTexturePack(const std::pair<uint32_t, uint32_t>& frames);

	bool isValid() const noexcept { return numFrames > 0; }

	void getTexture(uint32_t index, TextureInfo& ti) const noexcept;

	uint32_t getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const;
};
