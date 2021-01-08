#include "TexturePack.h"
#include "AnimationInfo.h"

void TexturePack::updateTextureRect(TextureInfo& ti)
{
	auto size = ti.texture->getSize();
	ti.textureRect.left = 0;
	ti.textureRect.top = 0;
	ti.textureRect.width = (int)size.x;
	ti.textureRect.height = (int)size.y;
}

uint32_t TexturePack::getTextureWidth(const sf::Texture& texture)
{
	return texture.getSize().x;
}

std::pair<uint32_t, uint32_t> TexturePack::getRange(uint32_t startIdx,
	uint32_t stopIdx, int32_t directionIdx, uint32_t directions)
{
	if (directions > 1 && directionIdx >= 0 && (uint32_t)directionIdx < directions)
	{
		auto animSize = stopIdx - startIdx;
		if (animSize % directions == 0)
		{
			auto frameSize = animSize / directions;
			return std::make_pair(
				startIdx + (frameSize * directionIdx),
				startIdx + (frameSize * (directionIdx + 1)) - 1
			);
		}
	}
	return std::make_pair(startIdx, stopIdx - 1);
}

bool TexturePack::get(uint32_t index, TextureInfoVar& tiVar) const
{
	TextureInfo ti;
	if (get(index, ti) == true)
	{
		tiVar.emplace<TextureInfo>(std::move(ti));
		return true;
	}
	return false;
}

AnimationInfo TexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.indexRange = std::make_pair((uint32_t)0, size() - 1);
	return animInfo;
}
