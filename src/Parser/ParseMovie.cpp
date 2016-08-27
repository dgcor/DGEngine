#include "ParseMovie.h"
#include "GameUtils.h"
#include <iostream>
#include "Movie2.h"
#include "ParseAction.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMovie(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false || isValidString(elem, "file") == false)
		{
			return;
		}

		std::shared_ptr<Action> action;
		if (elem.HasMember("onComplete"))
		{
			action = parseAction(game, elem["onComplete"]);
		}
		auto movie = std::make_shared<Movie2>(elem["file"].GetString());
		if (movie->load() == false)
		{
			if (action != nullptr)
			{
				game.Events().addBack(action);
			}
			return;
		}
		else
		{
			if (action != nullptr)
			{
				movie->setActionComplete(action);
			}
		}

		auto anchor = getAnchorKey(elem, "anchor");
		movie->setAnchor(anchor);
		auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
		auto size = getVector2iKey<sf::Vector2f>(elem, "size", movie->Size());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		movie->Position(pos);
		movie->Size(size);

		auto volume = getVariableKey(elem, "volume");
		auto vol = game.getVarOrProp<int64_t, unsigned>(volume, game.MusicVolume());
		if (vol > 100)
		{
			vol = 100;
		}
		movie->setVolume((float)vol);

		movie->Visible(getBoolKey(elem, "visible", true));

		game.Resources().addDrawable(elem["id"].GetString(), movie);

		try
		{
			movie->play();
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what();
		}
	}
}
