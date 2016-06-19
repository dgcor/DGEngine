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

Variable Circle::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			return GameUtils::getProperty(*this, Utils::str2int(props[0].c_str()), props);
		}
	}
	return Variable();
}
