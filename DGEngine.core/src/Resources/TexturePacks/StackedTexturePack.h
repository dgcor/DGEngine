#pragma once

#include "Resources/TexturePack.h"
#include <vector>

class StackedTexturePack : public TexturePack
{
protected:
	std::vector<std::shared_ptr<TexturePack>> texturePacks;
	std::shared_ptr<Palette> palette;
	std::pair<uint32_t, uint32_t> frameRange;

public:
	void addTexturePack(const std::shared_ptr<TexturePack>& texturePack);

	auto& geFrameRange() const noexcept { return frameRange; }

	bool get(uint32_t index, TextureInfo& ti) const override;

	sf::Vector2i getTextureSize(uint32_t index) const override;

	const sf::Texture* getTexture() const noexcept override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override;

	uint32_t size() const noexcept override;

	uint32_t getGroupCount() const noexcept override;

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
