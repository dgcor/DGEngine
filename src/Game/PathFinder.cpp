#include "PathFinder.h"
#include <cmath>
#include "LevelMap.h"

bool MapSearchNode::IsPassableIgnoreObject()
{
	if (x < map->Width() && y < map->Height())
	{
		return (*map)[(Coord)x][(Coord)y].PassableIgnoreObject();
	}
	return false;
}

bool MapSearchNode::IsPassable(int16_t x_, int16_t y_)
{
	if (x_ < map->Width() && y_ < map->Height())
	{
		return (*map)[(Coord)x_][(Coord)y_].Passable();
	}
	return false;
}

bool MapSearchNode::IsSameState(MapSearchNode& rhs)
{
	return ((x == rhs.x) && (y == rhs.y));
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode& nodeGoal)
{
	return (float)(std::abs(x - nodeGoal.x) + std::abs(y - nodeGoal.y));
}

bool MapSearchNode::IsGoal(MapSearchNode& nodeGoal)
{
	return ((x == nodeGoal.x) && (y == nodeGoal.y));
}

bool MapSearchNode::addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
	int16_t x_, int16_t y_, int16_t parent_x, int16_t parent_y)
{
	if ((IsPassable(x_, y_) == true) && !((parent_x == x_) && (parent_y == y_)))
	{
		auto direction_ = getPlayerDirection(
			MapCoord((Coord)parent_x, (Coord)parent_y),
			MapCoord((Coord)x_, (Coord)y_));
		auto searchNode = MapSearchNode(map, x_, y_, direction_);
		astarsearch->AddSuccessor(searchNode);
		return true;
	}
	return false;
}

bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode>* astarsearch, MapSearchNode* parent_node)
{
	int16_t parent_x = -1;
	int16_t parent_y = -1;

	if (parent_node != nullptr)
	{
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}

	bool canWalkLeft = addSuccessor(astarsearch, x - 1, y, parent_x, parent_y);
	bool canWalkRight = addSuccessor(astarsearch, x + 1, y, parent_x, parent_y);
	bool canWalkUp = addSuccessor(astarsearch, x, y - 1, parent_x, parent_y);
	bool canWalkDown = addSuccessor(astarsearch, x, y + 1, parent_x, parent_y);

	if (canWalkLeft == true)
	{
		if (canWalkUp == true)
		{
			addSuccessor(astarsearch, x - 1, y - 1, parent_x, parent_y);
		}
		if (canWalkDown == true)
		{
			addSuccessor(astarsearch, x - 1, y + 1, parent_x, parent_y);
		}
	}
	if (canWalkRight == true)
	{
		if (canWalkUp == true)
		{
			addSuccessor(astarsearch, x + 1, y - 1, parent_x, parent_y);
		}
		if (canWalkDown == true)
		{
			addSuccessor(astarsearch, x + 1, y + 1, parent_x, parent_y);
		}
	}
	return true;
}

float MapSearchNode::GetCost(MapSearchNode& successor)
{
	if (IsPassable(x, y) == true)
	{
		auto obj = (*map)[(Coord)x][(Coord)y].object.get();
		if (obj == nullptr)
		{
			return 1.f;
		}
		//else if (obj->Passable() == true)
		//{
		//	return 1.f;
		//}
	}
	return 9.f;
}
