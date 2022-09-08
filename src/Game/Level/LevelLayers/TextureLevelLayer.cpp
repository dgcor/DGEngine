#include "TextureLevelLayer.h"
#include <cmath>
#include "Game/Level/LevelSurface.h"
#include <SFML/Graphics/Sprite.hpp>

void TextureLevelLayer::update(sf::Time elapsedTime_)
{
	if (texture == nullptr ||
		parallaxElapsedTime.timeout == sf::Time::Zero ||
		parallaxElapsedTime.update(elapsedTime_) == false)
	{
		return;
	}

	parallaxFixedSpeedOffset -= parallaxFixedSpeed;

	auto size = texture->getSize();
	if (parallaxFixedSpeedOffset.x >= (float)size.x)
	{
		parallaxFixedSpeedOffset.x -= (float)size.x;
	}
	else if (parallaxFixedSpeedOffset.x <= 0.f)
	{
		parallaxFixedSpeedOffset.x += (float)size.x;
	}
	if (parallaxFixedSpeedOffset.y >= (float)size.y)
	{
		parallaxFixedSpeedOffset.y -= (float)size.y;
	}
	else if (parallaxFixedSpeedOffset.y <= 0.f)
	{
		parallaxFixedSpeedOffset.y += (float)size.y;
	}
}

void TextureLevelLayer::draw(const LevelSurface& surface) const
{
	if (texture == nullptr)
	{
		return;
	}
	auto rect = textureRect;
	if (rect.left == 0 && rect.top == 0 &&
		rect.width == 0 && rect.height == 0)
	{
		rect.left = (int)surface.visibleRect.left;
		rect.top = (int)surface.visibleRect.top;
		rect.width = (int)surface.visibleRect.width;
		rect.height = (int)surface.visibleRect.height;

		auto size = texture->getSize();
		if (parallaxSpeed != 0.f)
		{
			rect.left += (int)(std::round((double)surface.getCenter().x * parallaxSpeed)) % size.x;
			rect.top += (int)(std::round((double)surface.getCenter().y * parallaxSpeed)) % size.y;
		}
		if (parallaxFixedSpeedOffset.x != 0.f)
		{
			rect.left += (int)std::round(parallaxFixedSpeedOffset.x) % size.x;
		}
		if (parallaxFixedSpeedOffset.y != 0.f)
		{
			rect.top += (int)std::round(parallaxFixedSpeedOffset.y) % size.y;
		}
	}
	sf::Sprite sprite(*texture, rect);
	sprite.setPosition({ surface.visibleRect.left, surface.visibleRect.top });
	surface.draw(sprite);
}
