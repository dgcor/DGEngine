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

		auto movie = std::make_shared<Movie2>(elem["file"].GetString());
		if (movie->load() == false)
		{
			return;
		}

		auto anchor = getAnchor(elem, "anchor");
		movie->setAnchor(anchor);
		auto pos = getVector2f<sf::Vector2f>(elem, "position");
		auto size = getVector2i<sf::Vector2f>(elem, "size", movie->Size());
		if (getBool(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		movie->Position(pos);
		movie->Size(size);

		auto volume = getVariable(elem, "volume");
		auto vol = game.getVariable<int64_t, unsigned>(volume, game.MusicVolume());
		if (vol > 100)
		{
			vol = 100;
		}
		movie->setVolume((float)vol);

		movie->Visible(getBool(elem, "visible", true));

		if (elem.HasMember("onComplete"))
		{
			movie->setActionComplete(parseAction(game, elem["onComplete"]));
		}

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
