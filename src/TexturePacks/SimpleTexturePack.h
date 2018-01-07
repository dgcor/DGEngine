#pragma once

#include "TexturePack.h"
#include <vector>

struct MultiTexture
{
	std::shared_ptr<sf::Texture> texture;
	size_t maxFrames{ 0 };
	size_t numFrames{ 0 };
	size_t subImageSizeX{ 0 };
	size_t subImageSizeY{ 0 };
	bool horizontalDirection{ false };
};

class SimpleTexturePack : public TexturePack
{
private:
	MultiTexture t;
	std::shared_ptr<Palette> palette;
	bool indexed{ false };

public:
	SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture, size_t xFrames_,
		size_t yFrames_, bool horizontalDirection_ = false,
		const std::shared_ptr<Palette>& palette_ = nullptr, bool isIndexed_ = false);

	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool getTextureSize(sf::Vector2i& textureSize) const;

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return 1; }
	virtual size_t totalSize() const { return t.numFrames; }
	virtual size_t size(size_t index) const { return t.numFrames; }
};

class SimpleMultiTexturePack : public TexturePack
{
private:
	std::vector<MultiTexture> texVec;
	size_t textureCount{ 0 };
	size_t numFrames{ 0 };
	std::shared_ptr<Palette> palette;
	bool indexed{ false };
	bool texturesHaveSameSize{ false };

	bool texturesHaveSameNumFrames() const { return numFrames != 0; }

public:
	SimpleMultiTexturePack(const std::shared_ptr<Palette>& palette_, bool isIndexed_ = false)
		: palette(palette_), indexed(isIndexed_) {}

	virtual bool get(size_t index,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool get(size_t indexX, size_t indexY,
		const sf::Texture** texture, sf::IntRect& textureRect) const;

	virtual bool getTextureSize(sf::Vector2i& textureSize) const;

	void addTexturePack(const std::shared_ptr<sf::Texture>& texture,
		size_t xFrames, size_t yFrames, bool horizontalDirection = false);

	size_t getTextureCount() const { return texVec.size(); }

	virtual const std::shared_ptr<Palette>& getPalette() const { return palette; }
	virtual bool isIndexed() const { return indexed; }
	virtual size_t packSize() const { return texVec.size(); }
	virtual size_t totalSize() const { return textureCount; }
	virtual size_t size(size_t index) const { return texVec[index].numFrames; }
};
