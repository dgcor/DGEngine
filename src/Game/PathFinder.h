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

	MapSearchNode() noexcept : map(nullptr), x(0), y(0), direction(PlayerDirection::All) {}
	MapSearchNode(const LevelMap* map_, int16_t x_, int16_t y_,
		const PlayerDirection& direction_) noexcept : map(map_), x(x_), y(y_), direction(direction_) {}

	bool IsValid() const noexcept;

	bool IsPassableIgnoreObject() const noexcept;
	bool IsPassable() const { return IsPassable(x, y); }
	bool IsPassable(int16_t x_, int16_t y_) const;

	float GoalDistanceEstimateC(const MapSearchNode& nodeGoal) const noexcept;
	float GetCost() const;

	float GoalDistanceEstimate(MapSearchNode& nodeGoal) noexcept;
	bool IsGoal(MapSearchNode& nodeGoal) noexcept;
	bool GetSuccessors(AStarSearch<MapSearchNode>* astarsearch, MapSearchNode* parent_node);
	float GetCost(MapSearchNode& successor);
	bool IsSameState(MapSearchNode& rhs) noexcept;
};

bool getNearestPassableEndNode(const MapSearchNode& start, MapSearchNode& end);

typedef AStarSearch<MapSearchNode> PathFinder;
