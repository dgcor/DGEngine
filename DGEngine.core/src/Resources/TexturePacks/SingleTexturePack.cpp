#include "SingleTexturePack.h"
#include "Game/AnimationInfo.h"
#include <limits>

SingleTexturePack::SingleTexturePack(TexturePackGroup&& t_,
	const std::shared_ptr<Palette>& palette_) : t(std::move(t_)), palette(palette_) {}

SingleTexturePack::SingleTexturePack(TexturePackGroup&& t_,
	const std::pair<uint32_t, uint32_t>& frames, const std::shared_ptr<Palette>& palette_)
	: t(std::move(t_)), palette(palette_)
{
	t.makeTexturePack(frames);
}

bool SingleTexturePack::fetchIndex(uint32_t& index) const
{
	if (t.numFrames == 0 || index < t.startIndex)
	{
		return false;
	}
	index -= t.startIndex;
	if (index >= t.numFrames)
	{
		return false;
	}
	return true;
}

bool SingleTexturePack::get(uint32_t index, TextureInfo& ti) const noexcept
{
	if (fetchIndex(index) == false)
	{
		return false;
	}
	t.getTexture(index, ti);
	ti.palette = palette;
	return true;
}

sf::Vector2i SingleTexturePack::getTextureSize(uint32_t index) const
{
	if (fetchIndex(index) == false)
	{
		return {};
	}
	return { (int32_t)t.subImageSizeX, (int32_t)t.subImageSizeY };
}

uint32_t SingleTexturePack::getDirection(uint32_t frameIdx) const noexcept
{
	auto direction = t.getDirection(frameIdx);
	if (direction != std::numeric_limits<uint32_t>::max())
	{
		return direction;
	}
	return 0;
}

AnimationInfo SingleTexturePack::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	AnimationInfo animInfo;
	animInfo.animType = t.animType;
	animInfo.refresh = t.refresh;
	if (directionIdx >= 0)
	{
		if (t.directionsVec.empty() == false)
		{
			if ((uint32_t)directionIdx < t.directionsVec.size())
			{
				animInfo.indexRange = t.directionsVec[directionIdx];
				return animInfo;
			}
		}
		else if ((uint32_t)directionIdx < t.directions)
		{
			animInfo.indexRange = TexturePack::getRange(
				t.startIndex,
				t.startIndex + t.numFrames,
				directionIdx,
				t.directions
			);
			return animInfo;
		}
	}
	animInfo.indexRange = std::make_pair(t.startIndex, t.startIndex + t.numFrames - 1);
	return animInfo;
}
