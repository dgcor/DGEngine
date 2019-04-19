#include "Rectangle.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void Rectangle::updateSize(const Game& game)
{
	auto pos = getPosition();
	auto size = getSize();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	setPosition(pos);
	setSize(size);
}

bool Rectangle::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return getUIObjProp(str2int16(props.first), props.second, var);
}
