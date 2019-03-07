#pragma once

#include "TexturePack.h"
#include <vector>

class RectTexturePack : public TexturePack
{
private:
	struct RectTexture
	{
		size_t index;
		sf::IntRect rect;
		sf::Vector2f offset;
		RectTexture(size_t index_, const sf::IntRect& rect_, const sf::Vector2f& offset_)
			: index(index_), rect(rect_), offset(offset_) {}
	};

	std::unique_ptr<TexturePack> texturePack;
	std::vector<RectTexture> rects;
	bool absoluteOffsets{ false };

public:
	RectTexturePack(std::unique_ptr<TexturePack> texturePack_, bool absoluteOffsets_)
		: texturePack(std::move(texturePack_)), absoluteOffsets(absoluteOffsets_) {}

	void addRect(size_t index, const sf::IntRect& rect, const sf::Vector2f& offset);

	virtual bool get(size_t index, TextureInfo& ti) const;

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return texturePack->getPalette(); }
	virtual bool isIndexed() const noexcept { return texturePack->isIndexed(); }
	virtual size_t size() const noexcept { return rects.size(); }
};
