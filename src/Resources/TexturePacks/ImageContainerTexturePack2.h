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

	ImageContainerTexturePack2(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;

	int32_t getFlags(uint32_t index, uint32_t subIndex) const override;

	static uint32_t normalizedDirection(uint32_t direction, uint32_t numberOfDirections);

	static uint32_t denormalizedDirection(uint32_t direction, uint32_t numberOfDirections);
};
