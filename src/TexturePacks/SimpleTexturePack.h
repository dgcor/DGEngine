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

	virtual bool get(uint32_t index, TextureInfo& ti) const noexcept;

	virtual const sf::Texture* getTexture() const noexcept { return t.texture.get(); }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return t.numFrames; }

	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept { return t.directions; }
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept;
	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;
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

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	void addTexturePack(const std::shared_ptr<sf::Texture>& texture,
		const std::pair<uint32_t, uint32_t>& frames, const sf::Vector2f& offset,
		uint32_t startIndex, uint32_t directions, bool horizontalDirection,
		AnimationType animType);

	virtual const sf::Texture* getTexture() const noexcept;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return textureCount; }

	virtual uint32_t getGroupCount() const noexcept { return texVec.size(); }
	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept;
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept;
	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;
};
