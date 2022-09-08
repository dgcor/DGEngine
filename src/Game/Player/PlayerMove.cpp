#include "PlayerMove.h"
#include "Game/Game.h"
#include "Game/Level/LevelMap.h"
#include "Game/Player/PlayerBase.h"

bool PlayerMove::MapPosition(PlayerBase& player, LevelMap& map, const PairFloat& pos)
{
	player.mapPosA = player.mapPosition;
	player.mapPosB = pos;
	bool success = false;
	if (player.mapPosition != pos)
	{
		success = player.updateMapPositionBack(map, pos);
	}
	if (success == true || map.isMapCoordValid(pos) == true)
	{
		player.updateDrawPosition(map);
	}
	return success;
}

bool PlayerMove::move(PlayerBase& player, LevelMap& map, const PairFloat& pos)
{
	if (player.mapPosition == pos ||
		player.playerStatus == PlayerStatus::Dead ||
		map.isMapCoordValid(pos) == false)
	{
		return false;
	}
	player.clearWalkPath();
	player.setStandAnimation();
	player.playerStatus = PlayerStatus::Stand;
	player.resetAnimationTime();
	player.mapPosA = player.mapPosB = pos;
	bool success = player.updateMapPositionBack(map, pos);
	if (success == true)
	{
		player.updateDrawPosition(map);
	}
	return success;
}

void PlayerMove::setWalkPath(PlayerBase& player, const std::vector<PairFloat>& walkPath_, bool doAction)
{
	if (walkPath_.empty() == true ||
		player.playerStatus == PlayerStatus::Dead)
	{
		return;
	}
	player.walkPath = walkPath_;
	player.executeActionOnDestination = doAction;
	player.playerStatus = PlayerStatus::Walk;
	if (player.walkPath.empty() == false)
	{
		player.mapPositionMoveTo = player.walkPath.front();
	}
}

void PlayerMove::Walk(PlayerBase& player, const LevelMap& map, const PlayerDirection direction, bool doAction)
{
	PairFloat a = player.mapPosition;
	PairFloat b = a;
	switch (direction)
	{
	case PlayerDirection::Front:
		b.x++;
		b.y++;
		break;
	case PlayerDirection::FrontLeft:
		b.y++;
		break;
	case PlayerDirection::Left:
		b.x--;
		b.y++;
		break;
	case PlayerDirection::BackLeft:
		b.x--;
		break;
	case PlayerDirection::Back:
		b.x--;
		b.y--;
		break;
	case PlayerDirection::BackRight:
		b.y--;
		break;
	case PlayerDirection::Right:
		b.x++;
		b.y--;
		break;
	case PlayerDirection::FrontRight:
		b.x++;
		break;
	default:
		break;
	}
	if (a != b &&
		map.isMapCoordValid(b) == true &&
		map[b].PassableIgnoreObject() == true)
	{
		std::vector<PairFloat> path;
		path.push_back(b);
		path.push_back(a);
		setWalkPath(player, path, doAction);
	}
}

void PlayerMove::updateWalkPathStep(PlayerBase& player, PairFloat& newMapPos)
{
	newMapPos.x -= (player.mapPosA.x - player.mapPosB.x) * player.currPositionStep;
	newMapPos.y -= (player.mapPosA.y - player.mapPosB.y) * player.currPositionStep;

	if (player.currPositionStep >= 1.f)
	{
		if (player.walkPath.empty() == false)
		{
			player.walkPath.pop_back();
		}
		player.mapPosA = player.mapPosB;
		newMapPos = player.mapPosB;
	}
	else
	{
		player.currPositionStep += player.positionStep;
	}
}

void PlayerMove::updateWalkPath(PlayerBase& player, Game& game, LevelMap& map)
{
	player.currentWalkTime += game.getElapsedTime();

	while (player.currentWalkTime >= player.speed.walk)
	{
		player.currentWalkTime -= player.speed.walk;

		auto newMapPos = player.mapPosA;
		if (player.mapPosA == player.mapPosB)
		{
			if (player.walkPath.empty() == true &&
				player.hasWalkingAnimation() == true)
			{
				player.setStandAnimation();
				player.resetAnimationTime();
				player.playerStatus = PlayerStatus::Stand;
			}
			while (player.walkPath.empty() == false)
			{
				const auto& nextMapPos = player.walkPath.back();
				if (player.walkPath.size() == 1)
				{
					const auto levelObj = map[nextMapPos].front();
					if (levelObj != nullptr)
					{
						if (player.executeActionOnDestination == true)
						{
							levelObj->executeAction(game);
						}
						if (player.executeActionOnDestination == true ||
							levelObj->Passable() == false)
						{
							player.walkPath.pop_back();

							player.setStandAnimation();
							player.resetAnimationTime();
							player.playerStatus = PlayerStatus::Stand;
							return;
						}
					}
				}
				if (nextMapPos == player.mapPosition)
				{
					player.walkPath.pop_back();
					continue;
				}
				player.playSound("walk");
				player.setWalkAnimation();
				player.setDirection(getPlayerDirection(player.mapPosition, nextMapPos));
				MapPosition(player, map, nextMapPos);
				player.positionStep = 1.6f / map.DefaultTileWidth();
				player.currPositionStep = player.positionStep;
				updateWalkPathStep(player, newMapPos);
				break;
			}
		}
		else
		{
			updateWalkPathStep(player, newMapPos);
		}
		player.updateDrawPosition(map, newMapPos);
	}
}
