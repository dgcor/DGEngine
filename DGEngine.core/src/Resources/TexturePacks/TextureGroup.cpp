#include "TextureGroup.h"
#include "Game/AnimationInfo.h"

bool TextureGroup::makeTexturePack(const std::pair<uint32_t, uint32_t>& frames)
{
	if (texture == nullptr)
	{
		return false;
	}
	numFrames = frames.first * frames.second;
	if (numFrames == 0)
	{
		return false;
	}

	auto imgSize = texture->getSize();

	if (imgSize.x == 0 ||
		imgSize.y == 0 ||
		frames.first > imgSize.x ||
		frames.second > imgSize.y ||
		imgSize.x % frames.first != 0 ||
		imgSize.y % frames.second != 0)
	{
		numFrames = 0;
		return false;
	}

	maxFrames = (verticalDirection == false ? frames.first : frames.second);
	subImageSizeX = imgSize.x / frames.first;
	subImageSizeY = imgSize.y / frames.second;

	return true;
}

void TextureGroup::getTexture(uint32_t index, TextureInfo& ti) const noexcept
{
	if (numFrames <= 1 && index == 0)
	{
		ti.textureRect.left = 0;
		ti.textureRect.top = 0;
	}
	else if (verticalDirection == false)
	{
		ti.textureRect.left = (int)((index % maxFrames) * subImageSizeX);
		ti.textureRect.top = (int)((index / maxFrames) * subImageSizeY);
	}
	else
	{
		ti.textureRect.left = (int)((index / maxFrames) * subImageSizeX);
		ti.textureRect.top = (int)((index % maxFrames) * subImageSizeY);
	}
	ti.textureRect.width = subImageSizeX;
	ti.textureRect.height = subImageSizeY;
	ti.texture = texture.get();
	ti.offset = offset;
	ti.absoluteOffset = false;
	ti.blendMode = BlendMode::Alpha;
	ti.nextIndex = -1;
}

uint32_t TextureGroup::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	flags = (frameIdx >= startIndex && frameIdx < startIndex + numFrames) ? AnimationFlags::Valid : AnimationFlags::Overflow;
	return 0;
}

AnimationInfo TextureGroup::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.indexRange = { startIndex, startIndex + numFrames - 1 };
	animInfo.flags = (groupIdx == 0 && directionIdx == 0) ? AnimationFlags::Valid : AnimationFlags::Overflow;
	return animInfo;
}
