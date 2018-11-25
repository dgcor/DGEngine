#include "VectorTexturePack.h"

bool VectorTexturePack::get(size_t index, TextureInfo& ti) const
{
	if (index >= textures.size() ||
		textures[index].getNativeHandle() == 0)
	{
		return false;
	}
	ti.texture = &textures[index];
	updateTextureInfo(ti);
	ti.offset += offset;
	ti.palette = palette;
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
