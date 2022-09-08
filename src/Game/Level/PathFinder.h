#pragma once

#include "Game/Properties/PlayerDirection.h"
#include <limits>
#include "stlastar.h"

class LevelMap;

template <class T>
class AStarMapSearch : public AStarSearch<T>
{
public:
	const LevelMap* map{ nullptr };

	static constexpr int MaxNodes = 256;

	AStarMapSearch(const LevelMap* map_) : AStarSearch<T>(MaxNodes), map(map_) {}
};

class MapSearchNode
{
private:
	static constexpr float NodeInvalid = std::numeric_limits<float>::max();
	static constexpr float NodeNotPassable = 9.f;
	static constexpr float NodePassable = 1.f;

	bool addSuccessor(AStarSearch<MapSearchNode>* astarsearch,
		int16_t x_, int16_t y_, int16_t parent_x, int16_t parent_y);

public:
	int16_t x;
	int16_t y;
	float cost;
	PlayerDirection direction;

	MapSearchNode() noexcept : x(0), y(0), cost(NodeNotPassable), direction(PlayerDirection::All) {}
	MapSearchNode(const LevelMap& map, int16_t x_, int16_t y_,
		const PlayerDirection& direction_) noexcept;

	bool IsValid() const noexcept;

	bool IsPassable() const;

	float GoalDistanceEstimateC(const MapSearchNode& nodeGoal) const noexcept;
	float GetCost() const;

	float GoalDistanceEstimate(MapSearchNode& nodeGoal) noexcept;
	bool IsGoal(MapSearchNode& nodeGoal) noexcept;
	bool GetSuccessors(AStarSearch<MapSearchNode>* astarsearch, MapSearchNode* parent_node);
	float GetCost(MapSearchNode& successor);
	bool IsSameState(MapSearchNode& rhs) noexcept;
};

bool getNearestPassableEndNode(const LevelMap& map, const MapSearchNode& start, MapSearchNode& end);

using PathFinder = AStarMapSearch<MapSearchNode>;
