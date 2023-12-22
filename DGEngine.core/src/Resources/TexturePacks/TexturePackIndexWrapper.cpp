#include "TexturePackIndexWrapper.h"

bool TexturePackIndexWrapper::get(uint32_t index, TextureInfo& ti) const
{
	if (translateIndex(index) == true)
	{
		return texturePack->get(index, ti);
	}
	return false;
}

sf::Vector2i TexturePackIndexWrapper::getTextureSize(uint32_t index) const
{
	if (translateIndex(index) == true)
	{
		return texturePack->getTextureSize(index);
	}
	return {};
}
