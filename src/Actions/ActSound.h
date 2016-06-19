#pragma once

#include "Action.h"
#include "Game.h"
#include <string>

class ActSoundPlay : public Action
{
private:
	std::string id;
	Variable volume;

public:
	ActSoundPlay(const std::string& id_, const Variable& volume_)
		: id(id_), volume(volume_) {}

	virtual bool execute(Game& game)
	{
		auto sndBuffer = game.Resources().getSound(id);
		if (sndBuffer != nullptr)
		{
			sf::Sound sound(*sndBuffer.get());
			auto vol = game.getVariable<int64_t, unsigned>(volume, game.SoundVolume());
			if (vol > 0)
			{
				if (vol > 100)
				{
					vol = 100;
				}
				sound.setVolume((float)vol);
				game.Resources().addPlayingSound(sound);
			}
		}
		return true;
	}
};
