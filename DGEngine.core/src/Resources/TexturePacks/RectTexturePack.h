#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "TexturePackWrapper.h"
#include <vector>

class RectTexturePack : public TexturePackWrapper
{
protected:
	struct RectTexture
	{
		uint32_t index;
		sf::IntRect rect;
		sf::Vector2f offset;
		RectTexture(uint32_t index_, const sf::IntRect& rect_, const sf::Vector2f& offset_)
			: index(index_), rect(rect_), offset(offset_) {}
	};

	std::vector<RectTexture> rects;
	bool absoluteOffsets{ false };

public:
	RectTexturePack(std::unique_ptr<TexturePack> texturePack_, bool absoluteOffsets_)
		: TexturePackWrapper(std::move(texturePack_)), absoluteOffsets(absoluteOffsets_) {}

	void addRect(uint32_t index, const sf::IntRect& rect, const sf::Vector2f& offset);
	void addRect(const sf::IntRect& rect, const sf::Vector2f& offset);

	bool get(uint32_t index, TextureInfo& ti) const override;

	sf::Vector2i getTextureSize(uint32_t index) const override;

	uint32_t size() const noexcept override;

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override;

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
