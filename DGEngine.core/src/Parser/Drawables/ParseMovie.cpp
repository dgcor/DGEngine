#include "ParseMovie.h"
#include "Game/Drawables/Movie2.h"
#include "Game/Game.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Movie> getMovieObj(Game& game, const Value& elem)
	{
		std::shared_ptr<Action> action;
		if (elem.HasMember("onComplete"sv))
		{
			action = getActionVal(game, elem["onComplete"sv]);
		}
		auto movie = std::make_shared<Movie>(getStringViewVal(elem["file"sv]));
		if (movie->load() == false)
		{
			game.Events().tryAddBack(action);
			return nullptr;
		}
		else
		{
			if (action != nullptr)
			{
				movie->setAction(str2int16("complete"), action);
			}
		}

		auto size = getSizeKey(elem, "size", movie->Size());
		parseDrawableProperties(game, elem, *movie, size);

		auto volume = getVariableKey(elem, "volume");
		auto vol = game.getVarOrProp<int64_t, unsigned>(volume, game.MusicVolume());
		if (vol > 100)
		{
			vol = 100;
		}
		movie->setVolume((float)vol);

		return movie;
	}

	static std::shared_ptr<UIObject> parseMovieFunc(Game& game, const Value& elem)
	{
		auto movie = getMovieObj(game, elem);
		if (movie != nullptr)
		{
			movie->play();
		}
		return movie;
	}

	void parseMovie(Game& game, const Value& elem)
	{
		parseValidIdFileAndAddDrawable(game, elem, parseMovieFunc);
	}
}
