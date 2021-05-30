#pragma once

#include "AnimationType.h"
#include <SFML/Graphics/Rect.hpp>
#include "TexturePack.h"
#include <vector>

class RectTexturePack : public TexturePack
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

	struct Group
	{
		uint32_t startIdx;
		uint32_t stopIdx;
		uint32_t directions;
		AnimationType animType;
		Group(uint32_t startIdx_, uint32_t stopIdx_, uint32_t directions_, AnimationType animType_)
			: startIdx(startIdx_), stopIdx(stopIdx_), directions(directions_), animType(animType_) {}
	};

	std::unique_ptr<TexturePack> texturePack;
	std::vector<RectTexture> rects;
	std::vector<Group> groups;
	bool absoluteOffsets{ false };

public:
	RectTexturePack(std::unique_ptr<TexturePack> texturePack_, bool absoluteOffsets_)
		: texturePack(std::move(texturePack_)), absoluteOffsets(absoluteOffsets_) {}

	void addRect(uint32_t index, const sf::IntRect& rect, const sf::Vector2f& offset);
	void addRect(const sf::IntRect& rect, const sf::Vector2f& offset);

	void addGroup(uint32_t startIdx, uint32_t stopIdx, uint32_t directions, AnimationType animType);

	bool get(uint32_t index, TextureInfo& ti) const override;

	int32_t getWidth(uint32_t index) const override;

	const sf::Texture* getTexture() const noexcept override { return texturePack->getTexture(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return texturePack->getPalette(); }
	uint32_t size() const noexcept override { return (uint32_t)rects.size(); }

	uint32_t getGroupCount() const noexcept override;
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
