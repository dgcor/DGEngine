#include "ImageUtils.h"
#include "Pcx.h"
#include "PhysFSStream.h"
#include "Utils.h"

sf::Image ImageUtils::loadImage(const std::string& fileName, const sf::Color& color)
{
	sf::Image image;

	if (Utils::endsWith(Utils::toLower(fileName), ".pcx"))
	{
		image = ImageUtils::LoadImagePCX(fileName.c_str());
	}
	else
	{
		sf::PhysFSStream file(fileName);
		if (file.hasError() == true)
		{
			return image;
		}
		image.loadFromStream(file);
	}

	if (color != sf::Color::Transparent)
	{
		image.createMaskFromColor(color);
	}

	return image;
}

sf::Image ImageUtils::splitImageHorizontal(const sf::Image& img, unsigned pieces)
{
	if (pieces <= 1)
	{
		return img;
	}

	auto size = img.getSize();

	if (pieces > size.x)
	{
		return img;
	}

	auto newX = size.x / pieces;

	if (size.x % pieces != 0)
	{
		return img;
	}

	auto newY = size.y * pieces;

	sf::Image newImg;
	newImg.create(newX, newY);

	sf::IntRect rect(0, 0, newX, size.y);

	for (auto i = 0u; i < pieces; i++)
	{
		rect.left = i * newX;
		newImg.copy(img, 0, i * size.y, rect);
	}

	return newImg;
}

sf::Image ImageUtils::splitImageVertical(const sf::Image& img, unsigned pieces)
{
	if (pieces <= 1)
	{
		return img;
	}

	auto size = img.getSize();

	if (pieces > size.y)
	{
		return img;
	}

	auto newY = size.y / pieces;

	if (size.y % pieces != 0)
	{
		return img;
	}

	auto newX = size.x * pieces;

	sf::Image newImg;
	newImg.create(newX, newY);

	sf::IntRect rect(0, 0, size.x, newY);

	for (auto i = 0u; i < pieces; i++)
	{
		rect.top = i * newY;
		newImg.copy(img, i * size.x, 0, rect);
	}

	return newImg;
}
