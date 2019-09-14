#pragma once

#include "TexturePack.h"
#include <unordered_map>
#include "Utils/ElapsedTime.h"
#include <vector>

class IndexedTexturePack : public TexturePack
{
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

	virtual bool get(uint32_t index, TextureInfo& ti) const;

	virtual void update(int epoch, sf::Time elapsedTime);

	TexturePack* getTexturePack() const noexcept { return texturePack.get(); }

	void addAnimatedTexture(uint32_t animIndex,
		sf::Time refresh, const std::vector<uint32_t>& indexes);

	void mapTextureIndex(uint32_t mapIndex);
	void mapTextureIndex(uint32_t mapIndex, uint32_t toIndex);

	virtual const sf::Texture* getTexture() const noexcept { return texturePack->getTexture(); }

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return texturePack->getPalette(); }
	virtual uint32_t size() const noexcept { return numIndexedTextures; }

	virtual uint32_t getGroupCount() const noexcept { return texturePack->getGroupCount(); }
	virtual uint32_t getDirectionCount(uint32_t groupIdx) const noexcept
	{
		return texturePack->getDirectionCount(groupIdx);
	}
	virtual uint32_t getDirection(uint32_t frameIdx) const noexcept
	{
		return texturePack->getDirection(frameIdx);
	}

	virtual int32_t getFlags(uint32_t index, uint32_t subIndex) const;
};
