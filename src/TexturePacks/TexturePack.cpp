#include "TexturePack.h"

void TexturePack::updateTextureRect(const sf::Texture& texture, sf::IntRect& textureRect)
{
	auto size = texture.getSize();
	textureRect.left = 0;
	textureRect.top = 0;
	textureRect.width = (int)size.x;
	textureRect.height = (int)size.y;
}
