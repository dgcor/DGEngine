#pragma once

#include "TexturePack.h"
#include <unordered_map>
#include "Utils/ElapsedTime.h"
#include <vector>

class IndexedTexturePack : public TexturePack
{
protected:
	struct TexturePackAnimation
	{
		ElapsedTime refresh;
		uint32_t currentIndex{ 0 };
		std::vector<uint32_t> indexes;

		uint32_t getCurrentAnimationIndex() const noexcept { return indexes[currentIndex]; }
	};

	std::unique_ptr<TexturePack> texturePack;
	std::vector<TexturePackAnimation> animatedTextures;
	std::unordered_map<uint32_t, uint32_t> animatedIndexes;
	std::unordered_map<uint32_t, uint32_t> textureIndexes;
	uint32_t numIndexedTextures{ 0 };
	bool onlyUseIndexed{ false };
	bool translateAnimatedIndexes{ false };
	int lastEpoch{ 0 };

	bool translateIndex(uint32_t& index) const;

public:
	IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_,
		bool onlyUseIndexed_, bool translateAnimatedIndexes_);

	bool get(uint32_t index, TextureInfo& ti) const override;

	int32_t getWidth(uint32_t index) const override;

	void update(int epoch, sf::Time elapsedTime) override;

	TexturePack* getTexturePack() const noexcept { return texturePack.get(); }

	void addAnimatedTexture(uint32_t animIndex,
		sf::Time refresh, const std::vector<uint32_t>& indexes);

	void mapTextureIndex(uint32_t mapIndex);
	void mapTextureIndex(uint32_t mapIndex, uint32_t toIndex);

	const sf::Texture* getTexture() const noexcept override { return texturePack->getTexture(); }

	const std::shared_ptr<Palette>& getPalette() const noexcept override { return texturePack->getPalette(); }
	uint32_t size() const noexcept override { return numIndexedTextures; }

	uint32_t getGroupCount() const noexcept override { return texturePack->getGroupCount(); }
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override
	{
		return texturePack->getDirectionCount(groupIdx);
	}
	uint32_t getDirection(uint32_t frameIdx) const noexcept override
	{
		return texturePack->getDirection(frameIdx);
	}
};
