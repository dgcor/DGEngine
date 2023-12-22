#include "StackedTexturePack.h"
#include "Game/AnimationInfo.h"

void StackedTexturePack::addTexturePack(const std::shared_ptr<TexturePack>& texturePack)
{
	if (texturePack == nullptr || texturePack->size() == 0)
	{
		return;
	}
	if (texturePacks.empty() == true)
	{
		auto animInfo = texturePack->getAnimation(-1, -1);
		frameRange = animInfo.indexRange;
	}
	else
	{
		auto animInfo = texturePack->getAnimation(-1, -1);
		if (animInfo.indexRange.first < frameRange.first)
		{
			frameRange.first = animInfo.indexRange.first;
		}
		if (animInfo.indexRange.second > frameRange.second)
		{
			frameRange.second = animInfo.indexRange.second;
		}
	}
	texturePacks.push_back(texturePack);
}

bool StackedTexturePack::get(uint32_t index, TextureInfo& ti) const
{
	for (const auto& texturePack : texturePacks)
	{
		if (texturePack->get(index, ti) == true)
		{
			return true;
		}
	}
	return false;
}

sf::Vector2i StackedTexturePack::getTextureSize(uint32_t index) const
{
	for (const auto& texturePack : texturePacks)
	{
		auto size = texturePack->getTextureSize(index);
		if (size.x > 0 && size.y > 0)
		{
			return size;
		}
	}
	return {};
}

const sf::Texture* StackedTexturePack::getTexture() const noexcept
{
	if (texturePacks.size() == 1)
	{
		return texturePacks.front()->getTexture();
	}
	return {};
}

const std::shared_ptr<Palette>& StackedTexturePack::getPalette() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getPalette();
	}
	return palette;
}

uint32_t StackedTexturePack::size() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return frameRange.second + 1 - frameRange.first;
	}
	return 0;
}

uint32_t StackedTexturePack::getGroupCount() const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getGroupCount();
	}
	return 1;
}

uint32_t StackedTexturePack::getDirectionCount(uint32_t groupIdx) const noexcept
{
	if (texturePacks.empty() == false)
	{
		return texturePacks.front()->getDirectionCount(groupIdx);
	}
	return 1;
}

std::pair<uint32_t, uint32_t> StackedTexturePack::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	for (const auto& texturePack : texturePacks)
	{
		auto groupDirection = texturePack->getDirection(frameIdx, flags);
		if ((int)(flags & AnimationFlags::Valid) != 0)
		{
			return groupDirection;
		}
	}
	flags = AnimationFlags::Invalid;
	return {};
}

AnimationInfo StackedTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	for (const auto& texturePack : texturePacks)
	{
		auto animInfo = texturePack->getAnimation(groupIdx, directionIdx);
		if (animInfo.Valid() == true)
		{
			return animInfo;
		}
	}
	AnimationInfo animInfo;
	animInfo.indexRange = frameRange;
	animInfo.flags = AnimationFlags::Overflow;
	return animInfo;
}
