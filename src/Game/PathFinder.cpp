#include "PathFinder.h"
#include <cmath>
#include "LevelMap.h"

bool MapSearchNode::IsPassableIgnoreObject()
{
	if (x < map->Width() && y < map->Height())
	{
		return (*map)[x][y].PassableIgnoreObject();
	}
	return false;
}

bool MapSearchNode::IsPassable(size_t x_, size_t y_)
{
	if (x_ < map->Width() && y_ < map->Height())
	{
		return (*map)[x_][y_].Passable();
	}
	return false;
}

bool MapSearchNode::IsSameState(MapSearchNode& rhs)
{
	return ((x == rhs.x) && (y == rhs.y));
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode& nodeGoal)
{
	return std::fabs(x - nodeGoal.x) + std::fabs(y - nodeGoal.y);
}

bool MapSearchNode::IsGoal(MapSearchNode& nodeGoal)
{
	return ((x == nodeGoal.x) && (y == nodeGoal.y));
}

bool MapSearchNode::addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
	int16_t x, int16_t y, int16_t parent_x, int16_t parent_y)
{
	if ((IsPassable(x, y) == true) && !((parent_x == x) && (parent_y == y)))
	{
		auto direction = getPlayerDirection(sf::Vector2i(parent_x, parent_y), sf::Vector2i(x, y));
		auto searchNode = MapSearchNode(map, x, y, direction);
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
		if ((*map)[x][y].object == nullptr)
		{
			return 1.f;
		}
	}
	return 9.f;
}
