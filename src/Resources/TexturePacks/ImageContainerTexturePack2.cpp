#include "ImageContainerTexturePack2.h"
#include "Resources/TexturePacks/TexturePackUtils.h"

ImageContainerTexturePack2::ImageContainerTexturePack2(
	const std::vector<std::shared_ptr<ImageContainer>>& imgVec_, const sf::Vector2f& offset_,
	const std::shared_ptr<Palette>& palette_, bool isIndexed_, bool normalizeDirections_) :
	ImageContainerTexturePack(imgVec_, offset_, palette_, isIndexed_),
	normalizeDirections(normalizeDirections_) {}

uint32_t ImageContainerTexturePack2::normalizedDirection(uint32_t direction, uint32_t numberOfDirections)
{
	if (numberOfDirections == 8 ||
		numberOfDirections == 16)
	{
		switch (direction)
		{
		case 0: return 4;
		case 1: return 0;
		case 2: return 5;
		case 3: return 1;
		case 4: return 6;
		case 5: return 2;
		case 6: return 7;
		case 7: return 3;
		default: return direction;
		}
	}
	return direction;
}

uint32_t ImageContainerTexturePack2::denormalizedDirection(uint32_t direction, uint32_t numberOfDirections)
{
	if (numberOfDirections == 8 ||
		numberOfDirections == 16)
	{
		switch (direction)
		{
		case 4: return 0;
		case 0: return 1;
		case 5: return 2;
		case 1: return 3;
		case 6: return 4;
		case 2: return 5;
		case 7: return 6;
		case 3: return 7;
		default: return direction;
		}
	}
	return direction;
}

ImageContainerTexturePack2::ImageContainerTexturePack2(const std::shared_ptr<ImageContainer>& imgPack_,
	const sf::Vector2f& offset_, const std::shared_ptr<Palette>& palette_, bool isIndexed_,
	bool normalizeDirections_) : ImageContainerTexturePack(imgPack_, offset_, palette_, isIndexed_),
	normalizeDirections(normalizeDirections_) {}

std::pair<uint32_t, uint32_t> ImageContainerTexturePack2::getDirection(uint32_t frameIdx, AnimationFlags& flags) const noexcept
{
	auto groupDirection = ImageContainerTexturePack::getDirection(frameIdx, flags);

	if ((int)(flags & AnimationFlags::Valid) != 0)
	{
		if (normalizeDirections == true)
		{
			groupDirection.second = denormalizedDirection(
				groupDirection.second,
				ImageContainerTexturePack::getDirectionCount(groupDirection.first)
			);
		}
	}
	return groupDirection;
}

AnimationInfo ImageContainerTexturePack2::getAnimation(int32_t groupIdx, int32_t directionIdx) const
{
	if (normalizeDirections == true &&
		groupIdx >= 0 &&
		(uint32_t)groupIdx < ImageContainerTexturePack::getGroupCount())
	{
		auto directions = ImageContainerTexturePack::getDirectionCount(groupIdx);
		if (directions > 1)
		{
			directionIdx = (int32_t)ImageContainerTexturePack2::normalizedDirection(
				directionIdx, directions
			);
		}
	}
	return ImageContainerTexturePack::getAnimation(groupIdx, directionIdx);
}

int32_t ImageContainerTexturePack2::getFlags(uint32_t index, uint32_t subIndex) const
{
	if (getImageContainers().empty() == false)
	{
		auto levelFlags = dynamic_cast<const LevelFlags*>(getImageContainers().front().get());
		if (levelFlags != nullptr)
		{
			return levelFlags->getFlags(index, subIndex);
		}
	}
	return 0;
}
