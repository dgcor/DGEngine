#include "PlayerProperties.h"

PlayerDirection getPlayerDirection(const sf::Vector2i& currPos, const sf::Vector2i& newPos)
{
	if (newPos.x == currPos.x)
	{
		if (newPos.y >= currPos.y)
		{
			return PlayerDirection::FrontLeft;
		}
		else
		{
			return PlayerDirection::BackRight;
		}
	}
	else if (newPos.x > currPos.x)
	{
		if (newPos.y == currPos.y)
		{
			return PlayerDirection::FrontRight;
		}
		else if (newPos.y > currPos.y)
		{
			return PlayerDirection::Front;
		}
		else
		{
			return PlayerDirection::Right;
		}
	}
	else
	{
		if (newPos.y == currPos.y)
		{
			return PlayerDirection::BackLeft;
		}
		else if (newPos.y > currPos.y)
		{
			return PlayerDirection::Left;
		}
		else
		{
			return PlayerDirection::Back;
		}
	}
}
