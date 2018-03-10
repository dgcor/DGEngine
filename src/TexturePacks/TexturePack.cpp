#include "TexturePack.h"

void TexturePack::updateTextureInfo(TextureInfo& ti)
{
	auto size = ti.texture->getSize();
	ti.textureRect.left = 0;
	ti.textureRect.top = 0;
	ti.textureRect.width = (int)size.x;
	ti.textureRect.height = (int)size.y;
	ti.offset.x = 0.f;
	ti.offset.y = 0.f;
}
