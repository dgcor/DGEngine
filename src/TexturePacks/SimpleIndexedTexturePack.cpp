#include "SimpleIndexedTexturePack.h"

bool SimpleIndexedMultiTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	auto it = textureIndexes.find(index);
	if (it == textureIndexes.cend())
	{
		return false;
	}
	return SimpleMultiTexturePack::get(it->second, texture, textureRect);
}

bool SimpleIndexedMultiTexturePack::get(size_t indexX, size_t indexY,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	return get(indexX, texture, textureRect);
}

void SimpleIndexedMultiTexturePack::addTextureIndex(size_t index)
{
	textureIndexes[index] = textureIndexes.size();
	if (numIndexedTextures < index + 1)
	{
		numIndexedTextures = index + 1;
	}
}
