#include "SimpleImageContainer.h"
#include "Game/Utils/ImageUtils.h"

SimpleImageContainer::SimpleImageContainer(sf::InputStream& file, uint32_t xFrames_,
	uint32_t yFrames_, uint32_t directions_, bool verticalDirection_,
	const sf::Color& transparencyMask) : verticalDirection(verticalDirection_)
{
	numFrames = xFrames_ * yFrames_;
	if (numFrames == 0)
	{
		return;
	}

	image = ImageUtils::loadImage(file, transparencyMask);

	auto imgSize = image.getSize();

	if (imgSize.x == 0 ||
		imgSize.y == 0 ||
		xFrames_ > imgSize.x ||
		yFrames_ > imgSize.y ||
		imgSize.x % xFrames_ != 0 ||
		imgSize.y % yFrames_ != 0)
	{
		numFrames = 0;
		return;
	}

	maxFrames = (verticalDirection_ == false ? xFrames_ : yFrames_);
	subImageSizeX = imgSize.x / xFrames_;
	subImageSizeY = imgSize.y / yFrames_;

	directions = directions_ > 0 ? directions_ : 1;
}

sf::Image2 SimpleImageContainer::get(uint32_t index,
	const PaletteArray* palette, ImageInfo& imgInfo) const
{
	sf::Image2 img;
	if (numFrames == 0 ||
		index >= numFrames)
	{
		return img;
	}

	imgInfo.offset = {};
	imgInfo.absoluteOffset = false;
	imgInfo.blendMode = blendMode;
	imgInfo.nextIndex = -1;

	img.create(subImageSizeX, subImageSizeY, sf::Color::Transparent);

	int left = 0;
	int top = 0;

	if (verticalDirection == false)
	{
		left = (int)((index % maxFrames) * subImageSizeX);
		top = (int)((index / maxFrames) * subImageSizeY);
	}
	else
	{
		left = (int)((index / maxFrames) * subImageSizeX);
		top = (int)((index % maxFrames) * subImageSizeY);
	}

	img.copy(image, 0, 0, sf::IntRect(left, top, (int)subImageSizeX, (int)subImageSizeY), true);

	if (palette != nullptr)
	{
		for (unsigned j = 0; j < subImageSizeY; j++)
		{
			for (unsigned i = 0; i < subImageSizeX; i++)
			{
				img.setPixel(i, j, (*palette)[img.getPixel(i, j).r]);
			}
		}
	}
	return img;
}

sf::Vector2u SimpleImageContainer::getImageSize(uint32_t index) const
{
	if (numFrames == 0 ||
		index >= numFrames)
	{
		return {};
	}
	return { subImageSizeX , subImageSizeY };
}
