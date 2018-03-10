#include "IndexedTexturePack.h"

IndexedTexturePack::IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_,
	bool onlyUseIndexed_) : texturePack(std::move(texturePack_)), onlyUseIndexed(onlyUseIndexed_)
{
	if (onlyUseIndexed == false)
	{
		numIndexedTextures = texturePack->size();
	}
}

bool IndexedTexturePack::get(size_t index, TextureInfo& ti) const
{
	auto it = textureIndexes.find(index);
	if (it != textureIndexes.cend())
	{
		index = it->second;
	}
	else if (onlyUseIndexed == true)
	{
		return false;
	}
	return texturePack->get(index, ti);
}

void IndexedTexturePack::mapTextureIndex(size_t mapIndex)
{
	mapTextureIndex(mapIndex, textureIndexes.size());
}

void IndexedTexturePack::mapTextureIndex(size_t mapIndex, size_t toIndex)
{
	textureIndexes[mapIndex] = toIndex;
	if (onlyUseIndexed == true)
	{
		numIndexedTextures = std::max(numIndexedTextures, toIndex + 1);
	}
	else
	{
		numIndexedTextures = std::max(numIndexedTextures, mapIndex + 1);
	}
}
