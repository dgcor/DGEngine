#pragma once

#include "Action.h"
#include "Game.h"
#include "Movie2.h"
#include <string>

class ActMoviePause : public Action
{
private:
	std::string id;

public:
	ActMoviePause(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Movie2>(id);
		if (item != nullptr)
		{
			item->pause();
		}
		return true;
	}
};

class ActMoviePlay : public Action
{
private:
	std::string id;

public:
	ActMoviePlay(const std::string& id_) : id(id_) {}

	virtual bool execute(Game& game)
	{
		auto item = game.Resources().getResource<Movie2>(id);
		if (item != nullptr)
		{
			item->play();
		}
		return true;
	}
};
