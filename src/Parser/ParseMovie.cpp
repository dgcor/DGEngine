#include "ParseMovie.h"
#include "Game.h"
#include "GameUtils.h"
#include <iostream>
#include "Movie2.h"
#include "Panel.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseMovie(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return;
		}

		auto file = getStringViewVal(elem["file"sv]);
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		else if (getIdFromFile(file, id) == false)
		{
			return;
		}
		if (isValidId(id) == false)
		{
			return;
		}

		std::shared_ptr<Action> action;
		if (elem.HasMember("onComplete"sv))
		{
			action = getActionVal(game, elem["onComplete"sv]);
		}
		auto movie = std::make_shared<Movie>(file);
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
				movie->setAction(str2int16("complete"), action);
			}
		}

		auto anchor = getAnchorKey(elem, "anchor");
		movie->setAnchor(anchor);
		auto size = getVector2fKey<sf::Vector2f>(elem, "size", movie->Size());
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
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

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(movie);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, movie, manageObjDrawing, getStringViewKey(elem, "resource")
		);

		movie->play();
	}
}
