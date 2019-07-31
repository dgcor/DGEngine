#pragma once

#include "ImageContainers/ImageContainer.h"
#include "TexturePack.h"
#include <vector>

class CachedTexturePack : public TexturePack
{
private:
	std::shared_ptr<ImageContainer> imgPack;
	sf::Vector2f offset;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	bool normalizeDirections{ false };

	mutable std::vector<std::pair<sf::Texture, ImageContainer::ImageInfo>> cache;

public:
	CachedTexturePack(const std::shared_ptr<ImageContainer>& imgPack_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return cache.size(); }

	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept;
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept;
	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;
};

class CachedMultiTexturePack : public TexturePack
{
private:
	std::vector<std::shared_ptr<ImageContainer>> imgVec;
	sf::Vector2f offset;
	uint32_t textureCount{ 0 };
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	bool normalizeDirections{ false };

	mutable std::vector<std::pair<sf::Texture, ImageContainer::ImageInfo>> cache;

public:
	CachedMultiTexturePack(const std::vector<std::shared_ptr<ImageContainer>>& imgVec_,
		const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_,
		bool isIndexed_, bool normalizeDirections_);

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual uint32_t size() const noexcept { return textureCount; }

	virtual uint32_t getGroupCount() const noexcept { return imgVec.size(); }
	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept;
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept;
	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;
};
