#pragma once

#include "TexturePack.h"
#include <vector>

struct MultiTexture
{
	std::shared_ptr<sf::Texture> texture;
	sf::Vector2f offset;
	uint32_t startIndex{ 0 };
	uint32_t maxFrames{ 0 };
	uint32_t numFrames{ 0 };
	uint32_t subImageSizeX{ 0 };
	uint32_t subImageSizeY{ 0 };
	uint32_t directions{ 0 };
	AnimationType animType{ AnimationType::Looped };
	bool horizontalDirection{ false };
};

class SimpleTexturePack : public TexturePack
{
private:
	MultiTexture t;
	std::shared_ptr<Palette> palette;

public:
	SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture,
		const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
		uint32_t startIndex, uint32_t directions_, bool horizontalDirection,
		AnimationType animType, const std::shared_ptr<Palette>& palette_);

	bool get(uint32_t index, TextureInfo& ti) const noexcept override;

	const sf::Texture* getTexture() const noexcept override { return t.texture.get(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }
	uint32_t size() const noexcept override { return t.numFrames; }

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override { return t.directions; }
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	std::pair<uint32_t, uint32_t> getRange(int32_t groupIdx,
		int32_t directionIdx, AnimationType& animType) const override;
};

class SimpleMultiTexturePack : public TexturePack
{
private:
	std::vector<MultiTexture> texVec;
	uint32_t textureCount{ 0 };
	uint32_t numFrames{ 0 };
	std::shared_ptr<Palette> palette;
	bool texturesHaveSameSize{ false };
	bool indexesHaveGaps{ false };

	bool texturesHaveSameNumFrames() const noexcept { return numFrames != 0; }

public:
	SimpleMultiTexturePack(const std::shared_ptr<Palette>& palette_) : palette(palette_) {}

	bool get(uint32_t index, TextureInfo& ti) const override;

	void addTexturePack(const std::shared_ptr<sf::Texture>& texture,
		const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
		uint32_t startIndex, uint32_t directions, bool horizontalDirection,
		AnimationType animType);

	const sf::Texture* getTexture() const noexcept override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }
	uint32_t size() const noexcept override { return textureCount; }

	uint32_t getGroupCount() const noexcept override { return texVec.size(); }
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	std::pair<uint32_t, uint32_t> getRange(int32_t groupIdx,
		int32_t directionIdx, AnimationType& animType) const override;
};
