#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseMoviePause(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseMoviePlay(const rapidjson::Value& elem);
}
