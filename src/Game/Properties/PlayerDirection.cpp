#include "PlayerDirection.h"

PlayerDirection getPlayerDirection(const PairFloat& currPos, const PairFloat& newPos) noexcept
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
