#pragma once

#include "PlayerProperties.h"
#include "stlastar.h"

#define PATH_FINDER_MAX 100

class LevelMap;

class MapSearchNode
{
private:
	bool addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
		int16_t x, int16_t y, int16_t parent_x, int16_t parent_y);

public:
	const LevelMap* map = nullptr;
	int16_t x;
	int16_t y;
	PlayerDirection direction;

	MapSearchNode() : map(nullptr), x(0), y(0), direction(PlayerDirection::All) {}
	MapSearchNode(const LevelMap* map_, int16_t x_, int16_t y_,
		const PlayerDirection& direction_) : map(map_), x(x_), y(y_), direction(direction_) {}

	bool IsPassableIgnoreObject();
	bool IsPassable() { return IsPassable((size_t)x, (size_t)y); }
	bool IsPassable(size_t x_, size_t y_);

	float GoalDistanceEstimate(MapSearchNode& nodeGoal);
	bool IsGoal(MapSearchNode& nodeGoal);
	bool GetSuccessors(AStarSearch<MapSearchNode>* astarsearch, MapSearchNode* parent_node);
	float GetCost(MapSearchNode& successor);
	bool IsSameState(MapSearchNode& rhs);
};

typedef AStarSearch<MapSearchNode> PathFinder;
