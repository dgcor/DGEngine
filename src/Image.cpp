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
}

bool Image::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first), props.second, var);
}
