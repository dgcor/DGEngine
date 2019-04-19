#include "IndexedTexturePack.h"
#include "TextureInfo.h"

IndexedTexturePack::IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_,
	bool onlyUseIndexed_) : texturePack(std::move(texturePack_)), onlyUseIndexed(onlyUseIndexed_)
{
	if (onlyUseIndexed == false)
	{
		numIndexedTextures = texturePack->size();
	}
}

bool IndexedTexturePack::getTexture(size_t index, TextureInfo& ti) const
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

bool IndexedTexturePack::get(size_t index, TextureInfo& ti) const
{
	auto it = animationIndexes.find(index);
	if (it != animationIndexes.cend())
	{
		index = animations[it->second].getCurrentAnimationIndex();
	}
	return getTexture(index, ti);
}

void IndexedTexturePack::update(sf::Time elapsedTime)
{
	for (auto& anim : animations)
	{
		if (anim.refresh.update(elapsedTime) == true)
		{
			if (anim.currentIndex + 1 < anim.indexes.size())
			{
				anim.currentIndex++;
			}
			else
			{
				anim.currentIndex = 0;
			}
		}
	}
}

void IndexedTexturePack::addAnimation(size_t animIndex,
	sf::Time refresh, const std::vector<size_t>& indexes)
{
	auto it = animationIndexes.find(animIndex);
	if (it != animationIndexes.cend())
	{
		return;
	}
	animationIndexes[animIndex] = animations.size();
	TexturePackAnimation anim;
	anim.refresh = refresh;
	anim.indexes = indexes;
	animations.push_back(anim);
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
