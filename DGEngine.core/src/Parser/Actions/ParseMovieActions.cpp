#include "ParseMovieActions.h"
#include "Game/Actions/ActMovie.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseMoviePause(const Value& elem)
	{
		return std::make_shared<ActMoviePause>(getStringViewKey(elem, "id"));
	}

	std::shared_ptr<Action> parseMoviePlay(const Value& elem)
	{
		return std::make_shared<ActMoviePlay>(getStringViewKey(elem, "id"));
	}
}
