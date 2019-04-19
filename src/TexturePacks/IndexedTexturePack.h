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
		size_t currentIndex{ 0 };
		std::vector<size_t> indexes;

		size_t getCurrentAnimationIndex() const noexcept { return indexes[currentIndex]; }
	};

	std::unique_ptr<TexturePack> texturePack;
	std::vector<TexturePackAnimation> animations;
	std::unordered_map<size_t, size_t> animationIndexes;
	std::unordered_map<size_t, size_t> textureIndexes;
	size_t numIndexedTextures{ 0 };
	bool onlyUseIndexed{ false };

	bool getTexture(size_t index, TextureInfo& ti) const;

public:
	IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_, bool onlyUseIndexed_);

	virtual bool get(size_t index, TextureInfo& ti) const;

	virtual void update(sf::Time elapsedTime);

	TexturePack* getTexturePack() const noexcept { return texturePack.get(); }

	void addAnimation(size_t animIndex,
		sf::Time refresh, const std::vector<size_t>& indexes);

	void mapTextureIndex(size_t mapIndex);
	void mapTextureIndex(size_t mapIndex, size_t toIndex);

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return texturePack->getPalette(); }
	virtual bool isIndexed() const noexcept { return texturePack->isIndexed(); }
	virtual size_t size() const noexcept { return numIndexedTextures; }
};
