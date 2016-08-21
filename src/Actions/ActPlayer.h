#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/PlayerProperties.h"
#include <string>

class ActPlayerMoveToClick : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;

public:
	ActPlayerMoveToClick(const std::string& idPlayer_,
		const std::string& idLevel_) : idPlayer(idPlayer_), idLevel(idLevel_) {}

	virtual bool execute(Game& game)
	{
		if (idPlayer.empty() == false)
		{
			auto level = game.Resources().getLevel(idLevel);
			if (level != nullptr)
			{
				auto player = level->getPlayer(idPlayer);
				if (player != nullptr)
				{
					auto a = player->MapPosition();
					auto b = level->getMapClickPosition(game);
					player->setWalkPath(level->Map().getPath(a, b));
				}
			}
		}
		return true;
	}
};

class ActPlayerSetPalette : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	size_t palette;

public:
	ActPlayerSetPalette(const std::string& idPlayer_,
		const std::string& idLevel_, size_t palette_)
		: idPlayer(idPlayer_), idLevel(idLevel_), palette(palette_) {}

	virtual bool execute(Game& game)
	{
		if (idPlayer.empty() == false)
		{
			auto level = game.Resources().getLevel(idLevel);
			if (level != nullptr)
			{
				auto player = level->getPlayer(idPlayer);
				if (player != nullptr)
				{
					player->setPalette(palette);
				}
			}
		}
		return true;
	}
};
