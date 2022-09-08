#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Movie;

namespace Parser
{
	std::shared_ptr<Movie> getMovieObj(Game& game, const rapidjson::Value& elem);

	void parseMovie(Game& game, const rapidjson::Value& elem);
}
