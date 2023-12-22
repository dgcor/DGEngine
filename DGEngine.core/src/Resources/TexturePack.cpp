#include "TexturePack.h"
#include "Game/AnimationInfo.h"

void TexturePack::updateTextureRect(TextureInfo& ti)
{
	auto size = ti.texture->getSize();
	ti.textureRect.left = 0;
	ti.textureRect.top = 0;
	ti.textureRect.width = (int)size.x;
	ti.textureRect.height = (int)size.y;
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

std::pair<uint32_t, uint32_t> TexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	flags = AnimationFlags::Invalid;
	return {};
}

uint32_t TexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	AnimationFlags flags;
	return getDirection(frameIdx, flags).second;
}

AnimationInfo TexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	auto size_ = size();
	animInfo.indexRange = { 0, size_ > 0 ? size_ - 1 : 0 };
	return animInfo;
}
