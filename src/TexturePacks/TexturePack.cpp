#include "TexturePack.h"
#include "TextureInfo.h"

void TexturePack::updateTextureRect(TextureInfo& ti)
{
	auto size = ti.texture->getSize();
	ti.textureRect.left = 0;
	ti.textureRect.top = 0;
	ti.textureRect.width = (int)size.x;
	ti.textureRect.height = (int)size.y;
}
