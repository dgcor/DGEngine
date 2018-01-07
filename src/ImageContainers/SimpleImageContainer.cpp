#include "SimpleImageContainer.h"
#include "ImageUtils.h"

SimpleImageContainer::SimpleImageContainer(const std::string& fileName, size_t xFrames_,
	size_t yFrames_, bool horizontalDirection_, const sf::Color& transparencyMask)
	: horizontalDirection(horizontalDirection_)
{
	numFrames = xFrames_ * yFrames_;
	if (numFrames == 0)
	{
		return;
	}

	image = ImageUtils::loadImage(fileName, transparencyMask);

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

	maxFrames = (horizontalDirection_ == true ? xFrames_ : yFrames_);
	subImageSizeX = imgSize.x / xFrames_;
	subImageSizeY = imgSize.y / yFrames_;
}

sf::Image2 SimpleImageContainer::get(size_t index, const PaletteArray* palette) const
{
	sf::Image2 img;
	if (numFrames == 0 ||
		index >= numFrames)
	{
		return img;
	}

	img.create(subImageSizeX, subImageSizeY, sf::Color::Transparent);

	int left = 0;
	int top = 0;

	if (horizontalDirection == true)
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
	return img;
}
