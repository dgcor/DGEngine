#include "PathFinder.h"
#include <cmath>
#include "LevelMap.h"
#include "Utils/StringHash.h"

MapSearchNode::MapSearchNode(const LevelMap& map, int16_t x_, int16_t y_,
	const PlayerDirection& direction_) noexcept : x(x_), y(y_), direction(direction_)
{
	if (map.isMapCoordValid(x_, y_) == true)
	{
		cost = map[x_][y_].Passable() ? NodePassable : NodeNotPassable;
	}
	else
	{
		cost = NodeInvalid;
	}
}

bool MapSearchNode::IsValid() const noexcept
{
	return cost != NodeInvalid;
}

bool MapSearchNode::IsPassable() const
{
	return cost == NodePassable;
}

bool MapSearchNode::IsSameState(MapSearchNode& rhs) noexcept
{
	return ((x == rhs.x) && (y == rhs.y));
}

size_t MapSearchNode::Hash() noexcept
{
	std::size_t hash = 0;
	hashCombine(hash, x);
	hashCombine(hash, y);
	hashCombine(hash, cost);
	hashCombine(hash, direction);
	return hash;
}

float MapSearchNode::GoalDistanceEstimateC(const MapSearchNode& nodeGoal) const noexcept
{
	return (float)(std::abs(x - nodeGoal.x) + std::abs(y - nodeGoal.y));
}

float MapSearchNode::GoalDistanceEstimate(MapSearchNode& nodeGoal) noexcept
{
	return GoalDistanceEstimateC(nodeGoal);
}

bool MapSearchNode::IsGoal(MapSearchNode& nodeGoal) noexcept
{
	return ((x == nodeGoal.x) && (y == nodeGoal.y));
}

bool MapSearchNode::addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
	int16_t x_, int16_t y_, int16_t parent_x, int16_t parent_y)
{
	const auto& map = *((PathFinder*)astarsearch)->map;

	if ((map.isMapCoordValid(x_, y_) == true) &&
		(map[(int32_t)x_][(int32_t)y_].Passable() == true) &&
		!((parent_x == x_) && (parent_y == y_)))
	{
		auto direction_ = getPlayerDirection(
			PairFloat((float)parent_x, (float)parent_y),
			PairFloat((float)x_, (float)y_));
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
	return cost;
}

float MapSearchNode::GetCost(MapSearchNode& successor)
{
	return GetCost();
}

const bool addNearestSuccessor(const LevelMap& map, std::vector<MapSearchNode>& neighbours,
	int16_t x_, int16_t y_, MapSearchNode parent)
{
	MapSearchNode neighbour(map, x_, y_, parent.direction);
	neighbour.x = x_;
	neighbour.y = y_;
	if ((neighbour.IsPassable() == true) && !((parent.x == x_) && (parent.y == y_)))
	{
		neighbours.push_back(neighbour);
		return true;
	}
	return false;
}

bool getNearestPassableEndNode(const LevelMap& map,
	const MapSearchNode& start, MapSearchNode& end)
{
	std::vector<MapSearchNode> neighbours;

	bool canWalkLeft = addNearestSuccessor(map, neighbours, end.x - 1, end.y, end);
	bool canWalkRight = addNearestSuccessor(map, neighbours, end.x + 1, end.y, end);
	bool canWalkUp = addNearestSuccessor(map, neighbours, end.x, end.y - 1, end);
	bool canWalkDown = addNearestSuccessor(map, neighbours, end.x, end.y + 1, end);

	if (canWalkLeft == true)
	{
		if (canWalkUp == true)
		{
			addNearestSuccessor(map, neighbours, end.x - 1, end.y - 1, end);
		}
		if (canWalkDown == true)
		{
			addNearestSuccessor(map, neighbours, end.x - 1, end.y + 1, end);
		}
	}
	if (canWalkRight == true)
	{
		if (canWalkUp == true)
		{
			addNearestSuccessor(map, neighbours, end.x + 1, end.y - 1, end);
		}
		if (canWalkDown == true)
		{
			addNearestSuccessor(map, neighbours, end.x + 1, end.y + 1, end);
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
