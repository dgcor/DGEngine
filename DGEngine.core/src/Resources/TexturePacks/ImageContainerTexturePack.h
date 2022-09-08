#pragma once

#include "Resources/ImageContainer.h"
#include "Resources/TexturePack.h"
#include <vector>

class ImageContainerTexturePack : public TexturePack
{
protected:
	std::shared_ptr<ImageContainer> imgPack;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

	mutable std::vector<std::pair<sf::Texture, ImageContainer::ImageInfo>> cache;

	bool fetchTexture(uint32_t index) const;

public:
	ImageContainerTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_);

	bool get(uint32_t index, TextureInfo& ti) const override;

	sf::Vector2i getTextureSize(uint32_t index) const override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }

	uint32_t size() const noexcept override { return (uint32_t)cache.size(); }

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
