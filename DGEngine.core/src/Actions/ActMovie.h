#pragma once

#include "Action.h"
#include "Game.h"
#include "Movie2.h"

class ActMoviePause : public Action
{
private:
	std::string id;

public:
	ActMoviePause(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		auto movie = game.Resources().getDrawable<Movie>(id);
		if (movie != nullptr)
		{
			movie->pause();
		}
		return true;
	}
};

class ActMoviePlay : public Action
{
private:
	std::string id;

public:
	ActMoviePlay(const std::string_view id_) : id(id_) {}

	bool execute(Game& game) override
	{
		auto movie = game.Resources().getDrawable<Movie>(id);
		if (movie != nullptr)
		{
			movie->play();
		}
		return true;
	}
};
