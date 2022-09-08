#pragma once

#include "Game/Properties/PlayerDirection.h"
#include "Utils/PairXY.h"
#include <vector>

class Game;
class LevelMap;
class PlayerBase;

class PlayerMove
{
public:
	static bool MapPosition(PlayerBase& player, LevelMap& map, const PairFloat& pos);

	static bool move(PlayerBase& player, LevelMap& map, const PairFloat& pos);

	static void setWalkPath(PlayerBase& player, const std::vector<PairFloat>& walkPath_, bool doAction);

	static void Walk(PlayerBase& player, const LevelMap& map, const PlayerDirection direction, bool doAction);

	static void updateWalkPathStep(PlayerBase& player, PairFloat& newMapPos);

	static void updateWalkPath(PlayerBase& player, Game& game, LevelMap& map);
};
