#pragma once

#include "Game/Level/LevelFlags.h"
#include "Resources/TexturePacks/MultiImageContainerTexturePack.h"

class MultiImageContainerTexturePack2 : public MultiImageContainerTexturePack
{
private:
	bool normalizeDirections{ false };

public:
	MultiImageContainerTexturePack2(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
