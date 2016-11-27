#pragma once

#include "Action.h"
#include "Game.h"
#include "Image.h"

class ActImageCenterTexture : public Action
{
private:
	std::string id;

public:
	ActImageCenterTexture(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Image>(id);
		if (item != nullptr)
		{
			item->centerTexture();
		}
		return true;
	}
};
