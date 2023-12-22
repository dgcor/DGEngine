#pragma once

#include "Game/AnimationInfo.h"
#include "Resources/TexturePack.h"

class TexturePackWrapper : public TexturePack
{
protected:
	std::unique_ptr<TexturePack> texturePack;

public:
	TexturePackWrapper(std::unique_ptr<TexturePack> texturePack_)
		: texturePack(std::move(texturePack_)) {}

	auto getTexturePack() const noexcept { return texturePack.get(); }

	bool get(uint32_t index, TextureInfo& ti) const override { return texturePack->get(index, ti); }

	sf::Vector2i getTextureSize(uint32_t index) const override { return texturePack->getTextureSize(index); }

	void update(int epoch, sf::Time elapsedTime) override { texturePack->update(epoch, elapsedTime); }

	const sf::Texture* getTexture() const noexcept override { return texturePack->getTexture(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return texturePack->getPalette(); }

	uint32_t size() const noexcept override { return texturePack->size(); }

	uint32_t getGroupCount() const noexcept override { return texturePack->getGroupCount(); }

	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override { return texturePack->getDirectionCount(groupIdx); }

	std::pair<uint32_t, uint32_t> getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept override
	{
		return texturePack->getDirection(frameIdx, flags);
	}

	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override
	{
		return texturePack->getAnimation(groupIdx, directionIdx);
	}
};
