#pragma once

#include "AnimationType.h"
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
	std::vector<std::pair<uint32_t, uint32_t>> directionsVec;
	AnimationType animType{ AnimationType::Looped };
	sf::Time refresh;
	bool horizontalDirection{ false };
};

class SingleTexturePack : public TexturePack
{
protected:
	MultiTexture t;
	std::shared_ptr<Palette> palette;

	bool fetchIndex(uint32_t& index) const;

	SingleTexturePack(MultiTexture&& t_, const std::shared_ptr<Palette>& palette_);

public:
	SingleTexturePack() = default;
	SingleTexturePack(MultiTexture&& t_, const std::pair<uint32_t, uint32_t>& frames,
		const std::shared_ptr<Palette>& palette_);

	bool get(uint32_t index, TextureInfo& ti) const noexcept override;

	int32_t getWidth(uint32_t index) const override;

	const sf::Texture* getTexture() const noexcept override { return t.texture.get(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }
	uint32_t size() const noexcept override { return t.numFrames; }

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override { return t.directions; }
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};

class SimpleTexturePack : public SingleTexturePack
{
public:
	SimpleTexturePack(MultiTexture&& t_, const std::shared_ptr<Palette>& palette_);

	void setSize(uint32_t size_) noexcept { t.numFrames = std::max(size_, 1u); }

	bool get(uint32_t index, TextureInfo& ti) const noexcept override;
};

class MultiTexturePack : public TexturePack
{
protected:
	std::vector<MultiTexture> texVec;
	uint32_t textureCount{ 0 };
	uint32_t numFrames{ 0 };
	std::shared_ptr<Palette> palette;
	bool texturesHaveSameSize{ false };
	bool indexesHaveGaps{ false };

	bool fetchIndex(uint32_t index, uint32_t& indexX, uint32_t& indexY) const;
	bool texturesHaveSameNumFrames() const noexcept { return numFrames != 0; }

public:
	MultiTexturePack(const std::shared_ptr<Palette>& palette_) : palette(palette_) {}

	bool get(uint32_t index, TextureInfo& ti) const override;

	int32_t getWidth(uint32_t index) const override;

	void addTexturePack(MultiTexture&& t, const std::pair<uint32_t, uint32_t>& frames);

	const sf::Texture* getTexture() const noexcept override;

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return palette; }
	uint32_t size() const noexcept override { return textureCount; }

	uint32_t getGroupCount() const noexcept override { return (uint32_t)texVec.size(); }
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
