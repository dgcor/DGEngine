#include "IndexedTexturePack.h"

IndexedTexturePack::IndexedTexturePack(std::unique_ptr<TexturePack> texturePack_,
	bool onlyUseIndexed_, bool translateAnimatedIndexes_) : texturePack(std::move(texturePack_)),
	onlyUseIndexed(onlyUseIndexed_), translateAnimatedIndexes(translateAnimatedIndexes_)
{
	if (onlyUseIndexed == false)
	{
		numIndexedTextures = texturePack->size();
	}
}

bool IndexedTexturePack::translateIndex(uint32_t& index) const
{
	bool translateIndex = true;
	auto it = animatedIndexes.find(index);
	if (it != animatedIndexes.cend())
	{
		index = animatedTextures[it->second].getCurrentAnimationIndex();
		translateIndex = translateAnimatedIndexes;
	}
	if (translateIndex == true)
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
	}
	return true;
}

bool IndexedTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (translateIndex(index) == true)
	{
		return texturePack->get(index, ti);
	}
	return false;
}

int32_t IndexedTexturePack::getWidth(uint32_t index) const
{
	if (translateIndex(index) == true)
	{
		return texturePack->getWidth(index);
	}
	return 0;
}

void IndexedTexturePack::update(int epoch, sf::Time elapsedTime)
{
	if (lastEpoch == epoch)
	{
		return;
	}
	lastEpoch = epoch;

	for (auto& anim : animatedTextures)
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

void IndexedTexturePack::addAnimatedTexture(uint32_t animIndex,
	sf::Time refresh, const std::vector<uint32_t>& indexes)
{
	auto it = animatedIndexes.find(animIndex);
	if (it != animatedIndexes.cend())
	{
		return;
	}
	animatedIndexes[animIndex] = (uint32_t)animatedTextures.size();
	TexturePackAnimation anim;
	anim.refresh = refresh;
	anim.indexes = indexes;
	animatedTextures.push_back(anim);
}

void IndexedTexturePack::mapTextureIndex(uint32_t mapIndex)
{
	mapTextureIndex(mapIndex, (uint32_t)textureIndexes.size());
}

void IndexedTexturePack::mapTextureIndex(uint32_t mapIndex, uint32_t toIndex)
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
