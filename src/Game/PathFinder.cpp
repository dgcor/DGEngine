#include "PathFinder.h"
#include <cmath>
#include "LevelMap.h"

bool MapSearchNode::IsValid() const
{
	return (x >= 0 &&
		x < map->Width() &&
		y >= 0 &&
		y < map->Height());
}

bool MapSearchNode::IsPassableIgnoreObject() const
{
	if (IsValid() == true)
	{
		return (*map)[(Coord)x][(Coord)y].PassableIgnoreObject();
	}
	return false;
}

bool MapSearchNode::IsPassable(int16_t x_, int16_t y_) const
{
	if (x_ >= 0 &&
		x_ < map->Width() &&
		y_ >= 0 &&
		y_ < map->Height())
	{
		return (*map)[(Coord)x_][(Coord)y_].Passable();
	}
	return false;
}

bool MapSearchNode::IsSameState(MapSearchNode& rhs)
{
	return ((x == rhs.x) && (y == rhs.y));
}

float MapSearchNode::GoalDistanceEstimateC(const MapSearchNode& nodeGoal) const
{
	return (float)(std::abs(x - nodeGoal.x) + std::abs(y - nodeGoal.y));
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode& nodeGoal)
{
	return GoalDistanceEstimateC(nodeGoal);
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

float MapSearchNode::GetCost() const
{
	if (IsPassable(x, y) == true)
	{
		return 1.f;
	}
	return 9.f;
}

float MapSearchNode::GetCost(MapSearchNode& successor)
{
	return GetCost();
}

const bool addNearestSuccessor(std::vector<MapSearchNode>& neighbours,
	int16_t x_, int16_t y_, MapSearchNode parent)
{
	MapSearchNode neighbour(parent);
	neighbour.x = x_;
	neighbour.y = y_;
	if ((neighbour.IsPassable() == true) && !((parent.x == x_) && (parent.y == y_)))
	{
		neighbours.push_back(neighbour);
		return true;
	}
	return false;
}

bool getNearestPassableEndNode(const MapSearchNode& start, MapSearchNode& end)
{
	std::vector<MapSearchNode> neighbours;

	bool canWalkLeft = addNearestSuccessor(neighbours, end.x - 1, end.y, end);
	bool canWalkRight = addNearestSuccessor(neighbours, end.x + 1, end.y, end);
	bool canWalkUp = addNearestSuccessor(neighbours, end.x, end.y - 1, end);
	bool canWalkDown = addNearestSuccessor(neighbours, end.x, end.y + 1, end);

	if (canWalkLeft == true)
	{
		if (canWalkUp == true)
		{
			addNearestSuccessor(neighbours, end.x - 1, end.y - 1, end);
		}
		if (canWalkDown == true)
		{
			addNearestSuccessor(neighbours, end.x - 1, end.y + 1, end);
		}
	}
	if (canWalkRight == true)
	{
		if (canWalkUp == true)
		{
			addNearestSuccessor(neighbours, end.x + 1, end.y - 1, end);
		}
		if (canWalkDown == true)
		{
			addNearestSuccessor(neighbours, end.x + 1, end.y + 1, end);
		}
	}

	std::sort(neighbours.begin(), neighbours.end(),
		[&start](const MapSearchNode& lhs, const MapSearchNode& rhs)
	{
		float costA = lhs.GetCost() + lhs.GoalDistanceEstimateC(start);
		float costB = rhs.GetCost() + rhs.GoalDistanceEstimateC(start);
		return costA < costB;
	});

	if (neighbours.empty() == true)
	{
		return false;
	}
	if (neighbours.front().IsPassable() == false)
	{
		return false;
	}
	end = neighbours.front();
	return true;
}
