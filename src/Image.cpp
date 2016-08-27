#include "Image.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

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

bool Image::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			return GameUtils::getUIObjProp(*this, Utils::str2int(props[0].c_str()), props, var);
		}
	}
	return false;
}
