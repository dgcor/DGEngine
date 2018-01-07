#include "VectorTexturePack.h"

bool VectorTexturePack::get(size_t index,
	const sf::Texture** texture, sf::IntRect& textureRect) const
{
	if (index >= textures.size() ||
		textures[index].getNativeHandle() == 0)
	{
		return false;
	}
	(*texture) = &textures[index];
	updateTextureRect(textures[index], textureRect);
	return true;
}

bool VectorTexturePack::getTextureSize(sf::Vector2i& textureSize) const
{
	if (texturesHaveSameSize() == false)
	{
		return false;
	}
	auto size = textures[lastTextureIndex].getSize();
	textureSize.x = (int)size.x;
	textureSize.y = (int)size.y;
	return true;
}

void VectorTexturePack::set(size_t index, const sf::Texture& texture)
{
	if (index < textures.size())
	{
		if (lastTextureIndex == textures.size())
		{
			lastTextureIndex = index;
		}
		else if (texturesHaveSameSize() == true)
		{
			if (textures[lastTextureIndex].getSize() == texture.getSize())
			{
				lastTextureIndex = index;
			}
			else
			{
				lastTextureIndex = std::numeric_limits<size_t>::max();
			}
		}
		textures[index] = texture;
	}
}
