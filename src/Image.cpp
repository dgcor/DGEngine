#include "Image.h"
#include "Game.h"
#include "GameUtils.h"
#include "SFML/SFMLUtils.h"
#include "Utils/Utils.h"

void Image::setOrigin()
{
	setOrigin(sf::Vector2f(std::round((float)sprite.getTextureRect().width / 2.f),
		std::round((float)sprite.getTextureRect().height / 2.f)));
}

void Image::centerTexture()
{
	SFMLUtils::spriteCenterTexture(sprite);
}

void Image::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = sprite.getPosition();
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	sprite.setPosition(pos);
	if (resizable == true)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}

sf::Vector2f Image::Size() const
{
	return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
}

void Image::Size(const sf::Vector2f& size)
{
	auto rect = sprite.getTextureRect();
	rect.width = (int)size.x;
	rect.height = (int)size.y;
	sprite.setTextureRect(rect);
}

void Image::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible)
	{
		sprite.draw(target, game.Shaders().Sprite);
	}
}

bool Image::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return getUIObjProp(str2int16(props.first), props.second, var);
}
