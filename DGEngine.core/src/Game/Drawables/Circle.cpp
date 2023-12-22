#include "Circle.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

Circle::Circle(float radius, std::size_t pointCount) : sf::CircleShape(std::round(radius), pointCount) {}

sf::Vector2f Circle::Size() const
{
	return sf::Vector2f(std::round(getLocalBounds().width), std::round(getLocalBounds().height));
}

void Circle::updateSize(const Game& game)
{
	auto pos = getPosition();
	auto size = Size();
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	setPosition(pos);
}

bool Circle::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("points"):
		var = Variable((int64_t)getPointCount());
		break;
	case str2int16("radius"):
		var = Variable((int64_t)getRadius());
		break;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
