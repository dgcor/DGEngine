#pragma once

#include "Json/JsonParser.h"
#include <memory>

class Game;
class Movie;

namespace Parser
{
	std::shared_ptr<Movie> getMovieObj(Game& game, const rapidjson::Value& elem,
		const std::string_view file);

	typedef decltype(&getMovieObj) getMovieObjFuncPtr;

	void parseMovie(Game& game, const rapidjson::Value& elem,
		const getMovieObjFuncPtr getMovieObjFunc);

	void parseMovie(Game& game, const rapidjson::Value& elem);
}
