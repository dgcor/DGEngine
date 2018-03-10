#pragma once

#include <map>
#include "TexturePack.h"

class IndexedTexturePack : public TexturePack
{
	std::unique_ptr<TexturePack> texturePack;
	std::map<size_t, size_t> textureIndexes;
	size_t numIndexedTextures{ 0 };
	bool onlyUseIndexed{ false };

public:
	IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_, bool onlyUseIndexed_);

	virtual bool get(size_t index, TextureInfo& ti) const;

	void mapTextureIndex(size_t mapIndex);
	void mapTextureIndex(size_t mapIndex, size_t toIndex);

	virtual const std::shared_ptr<Palette>& getPalette() const noexcept { return texturePack->getPalette(); }
	virtual bool isIndexed() const noexcept { return texturePack->isIndexed(); }
	virtual size_t size() const noexcept { return numIndexedTextures; }
};
