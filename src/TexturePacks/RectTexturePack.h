#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "TexturePack.h"
#include <vector>

class RectTexturePack : public TexturePack
{
private:
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

	void addGroup(uint32_t startIdx, uint32_t stopIdx, uint32_t directions, AnimationType animType);

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual const sf::Texture* getTexture() const noexcept { return texturePack->getTexture(); }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return texturePack->getPalette(); }
	virtual uint32_t size() const noexcept { return rects.size(); }

	virtual uint32_t getGroupCount() const noexcept;
	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept;
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept;
	virtual std::pair<uint32_t, uint32_t> getRange(
		int32_t groupIdx, int32_t directionIdx, AnimationType& animType) const;
};
