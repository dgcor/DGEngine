#pragma once

#include "Action.h"
#include "Game.h"
#include "Game/Level.h"
#include <string>

class ActPlayerSetCelTexture : public Action
{
private:
	std::string idPlayer;
	std::string idLevel;
	std::string idCelTex;

public:
	ActPlayerSetCelTexture(const std::string& idPlayer_,
		const std::string& idLevel_, const std::string& idCelTex_)
		: idPlayer(idPlayer_), idLevel(idLevel_), idCelTex(idCelTex_) {}

	virtual bool execute(Game& game)
	{
		if (idPlayer.size() > 0)
		{
			auto level = game.Resources().getLevel(idLevel);
			if (level != nullptr)
			{
				auto celText = game.Resources().getCelTextureCache(idCelTex);
				if (celText != nullptr)
				{
					auto player = level->getPlayer(idPlayer);
					if (player != nullptr)
					{
						player->setCelTexture(celText);
					}
				}
			}
		}
		return true;
	}
};
