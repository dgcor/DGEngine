#pragma once

#include "Game/Level/LevelFlags.h"
#include "Resources/TexturePacks/ImageContainerTexturePack.h"

class ImageContainerTexturePack2 : public ImageContainerTexturePack, public LevelFlags
{
private:
	bool normalizeDirections{ false };

public:
	ImageContainerTexturePack2(const std::shared_ptr<ImageContainer>& imgPack_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;

	int32_t getFlags(uint32_t index, uint32_t subIndex) const override;

	static uint32_t getNormalizedDirection(uint32_t direction, uint32_t numberOfDirections);
};
