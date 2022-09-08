#pragma once

#include "TexturePackGroup.h"

class SingleTexturePack : public TexturePack
{
protected:
	TexturePackGroup t;
	std::shared_ptr<Palette> palette;

	bool fetchIndex(uint32_t& index) const;

	SingleTexturePack(TexturePackGroup&& t_, const std::shared_ptr<Palette>& palette_);

public:
	SingleTexturePack() = default;
	SingleTexturePack(TexturePackGroup&& t_, const std::pair<uint32_t, uint32_t>& frames, const std::shared_ptr<Palette>& palette_);

	bool get(uint32_t index, TextureInfo& ti) const noexcept override;

	sf::Vector2i getTextureSize(uint32_t index) const override;

	const sf::Texture* getTexture() const noexcept override { return t.texture.get(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }

	uint32_t size() const noexcept override { return t.numFrames; }

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override { return t.directions; }

	uint32_t getDirection(uint32_t frameIdx) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
