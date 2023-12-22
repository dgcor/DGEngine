#pragma once

#include "TextureGroup.h"
#include "Utils/small_vector.hpp"

class MultiTexturePack : public TexturePack
{
protected:
	gch::small_vector<TextureGroup, 1> textureGroups;
	uint32_t textureCount{ 0 };
	uint32_t numFrames{ 0 };
	std::pair<uint32_t, uint32_t> frameRange;
	std::shared_ptr<Palette> palette;
	bool texturesHaveSameSize{ false };
	bool indexesHaveGaps{ false };

	bool fetchIndex(uint32_t index, uint32_t& indexX, uint32_t& indexY) const;
	bool texturesHaveSameNumFrames() const noexcept { return numFrames != 0; }

public:
	MultiTexturePack(const std::shared_ptr<Palette>& palette_) : palette(palette_) {}
	MultiTexturePack(TextureGroup&& textureGroup_, const std::shared_ptr<Palette>& palette_);

	bool get(uint32_t index, TextureInfo& ti) const override;

	sf::Vector2i getTextureSize(uint32_t index) const override;

	void addTextureGroup(TextureGroup&& textureGroup);

	const sf::Texture* getTexture() const noexcept override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }

	uint32_t size() const noexcept override { return textureCount; }

	uint32_t getGroupCount() const noexcept override;

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
