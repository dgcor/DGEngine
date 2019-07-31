#include "VectorTexturePack.h"
#include "TextureInfo.h"

bool VectorTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	if (index >= textures.size() ||
		textures[index].getNativeHandle() == 0)
	{
		return false;
	}
	ti.texture = &textures[index];
	updateTextureRect(ti);
	ti.offset = offset;
	ti.absoluteOffset = false;
	ti.blendMode = BlendMode::Alpha;
	ti.palette = palette;
	return true;
}

void VectorTexturePack::set(uint32_t index, const sf::Texture& texture)
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
				lastTextureIndex = std::numeric_limits<uint32_t>::max();
			}
		}
		textures[index] = texture;
	}
}
