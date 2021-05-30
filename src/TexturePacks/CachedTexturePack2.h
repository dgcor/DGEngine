#pragma once

#include "Game/LevelFlags.h"
#include "TexturePacks/CachedTexturePack.h"

class CachedTexturePack2 : public CachedTexturePack, public LevelFlags
{
private:
	bool normalizeDirections{ false };

public:
	CachedTexturePack2(const std::shared_ptr<ImageContainer>& imgPack_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;

	int32_t getFlags(uint32_t index, uint32_t subIndex) const override;
};

class CachedMultiTexturePack2 : public CachedMultiTexturePack
{
private:
	bool normalizeDirections{ false };

public:
	CachedMultiTexturePack2(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
