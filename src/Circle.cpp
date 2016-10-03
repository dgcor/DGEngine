#include "Circle.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void Circle::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = getPosition();
	auto size = sf::Vector2f(this->getLocalBounds().width, this->getLocalBounds().height);
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	setPosition(pos);
}

bool Circle::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitString(prop, '.');
	if (props.empty() == true)
	{
		return false;
	}
	return GameUtils::getUIObjProp(*this, Utils::str2int(props[0].c_str()), props, var);
}
