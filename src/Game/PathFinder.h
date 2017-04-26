#pragma once

#include "GameProperties.h"
#include "stlastar.h"

#define PATH_FINDER_MAX 150

class LevelMap;

class MapSearchNode
{
private:
	bool addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
		int16_t x_, int16_t y_, int16_t parent_x, int16_t parent_y);

public:
	const LevelMap* map = nullptr;
	int16_t x;
	int16_t y;
	PlayerDirection direction;

	MapSearchNode() : map(nullptr), x(0), y(0), direction(PlayerDirection::All) {}
	MapSearchNode(const LevelMap* map_, int16_t x_, int16_t y_,
		const PlayerDirection& direction_) : map(map_), x(x_), y(y_), direction(direction_) {}

	bool IsValid() const;

	bool IsPassableIgnoreObject() const;
	bool IsPassable() const { return IsPassable(x, y); }
	bool IsPassable(int16_t x_, int16_t y_) const;

	float GoalDistanceEstimateC(const MapSearchNode& nodeGoal) const;
	float GetCost() const;

	float GoalDistanceEstimate(MapSearchNode& nodeGoal);
	bool IsGoal(MapSearchNode& nodeGoal);
	bool GetSuccessors(AStarSearch<MapSearchNode>* astarsearch, MapSearchNode* parent_node);
	float GetCost(MapSearchNode& successor);
	bool IsSameState(MapSearchNode& rhs);
};

bool getNearestPassableEndNode(const MapSearchNode& start, MapSearchNode& end);

typedef AStarSearch<MapSearchNode> PathFinder;
