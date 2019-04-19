#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "TexturePack.h"
#include <vector>

struct MultiTexture
{
	std::shared_ptr<sf::Texture> texture;
	sf::Vector2f offset;
	size_t startIndex{ 0 };
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
	SimpleTexturePack(const std::shared_ptr<sf::Texture>& texture,
		const std::pair<size_t, size_t>& frames, const sf::Vector2f& offset,
		size_t startIndex, bool horizontalDirection,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_);

	virtual bool get(size_t index, TextureInfo& ti) const noexcept;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return t.numFrames; }
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
	bool indexesHaveGaps{ false };

	bool texturesHaveSameNumFrames() const noexcept { return numFrames != 0; }

public:
	SimpleMultiTexturePack(const std::shared_ptr<Palette>& palette_, bool isIndexed_)
		: palette(palette_), indexed(isIndexed_) {}

	virtual bool get(size_t index, TextureInfo& ti) const;

	void addTexturePack(const std::shared_ptr<sf::Texture>& texture,
		const std::pair<size_t, size_t>& frames, const sf::Vector2f& offset,
		size_t startIndex, bool horizontalDirection);

	size_t getTextureCount() const noexcept { return texVec.size(); }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	virtual bool isIndexed() const noexcept { return indexed; }
	virtual size_t size() const noexcept { return textureCount; }
};
