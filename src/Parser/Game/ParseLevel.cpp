#include "ParseLevel.h"
#include "Cel.h"
#include "FileUtils.h"
#include "Game/LevelMap.h"
#include "GameUtils.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDun(const Value& elem, LevelMap& map, const TileSet& til, const Sol& sol)
	{
		Dun dun(getStringKey(elem, "file"));
		if (dun.Width() > 0 && dun.Height() > 0)
		{
			auto pos = getVector2uKey<MapCoord>(elem, "position");
			map.setArea(pos.x, pos.y, dun, til, sol);
		}
	}

	void parseLevelMap(Game& game, const Value& elem, Level& level)
	{
		auto celPath = elem["cel"].GetString();
		auto tilPath = elem["til"].GetString();
		auto minPath = elem["min"].GetString();
		auto solPath = elem["sol"].GetString();
		auto palPath = elem["palette"].GetString();

		TileSet til(tilPath);
		if (til.size() == 0)
		{
			return;
		}
		Sol sol(solPath);
		if (sol.size() == 0)
		{
			return;
		}

		auto mapSize = getVector2uKey<MapCoord>(elem, "mapSize");
		LevelMap map(mapSize.x, mapSize.y);

		const auto& dunElem = elem["dun"];
		if (dunElem.IsArray() == true)
		{
			for (const auto& val : dunElem)
			{
				parseDun(val, map, til, sol);
			}
		}
		else if (dunElem.IsObject() == true)
		{
			parseDun(dunElem, map, til, sol);
		}

		// l4.min and town.min contain 16 blocks, all others 10.
		Min min(minPath, getIntKey(elem, "minBlocks", 10));
		if (min.size() == 0)
		{
			return;
		}

		auto pal = game.Resources().getPalette(palPath);
		if (pal == nullptr)
		{
			return;
		}
		bool isCl2 = Utils::endsWith(celPath, "cl2");
		CelFile cel(celPath, isCl2, true);

		CelFrameCache celCache(cel, *pal);
		level.Init(map, min, celCache);
		level.updateLevelObjectPositions();
	}

	void parsePosSize(Game& game, const Value& elem, Level& level)
	{
		auto anchor = getAnchorKey(elem, "anchor");
		level.setAnchor(anchor);
		auto pos = getVector2fKey<sf::Vector2f>(elem, "position");
		auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.WindowTexSizef());
		if (getBoolKey(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		level.Position(pos);
		level.Size(size);
		level.Zoom(((float)getIntKey(elem, "zoom", 100)) / 100.f);
		level.Visible(getBoolKey(elem, "visible", true));
	}

	void parseLevel(Game& game, const Value& elem)
	{
		auto id = getStringKey(elem, "id");
		Level* level = game.Resources().getResource<Level>(id);
		bool existingLevel = (level != nullptr);
		if (level == nullptr)
		{
			if (isValidId(id) == false)
			{
				return;
			}
			auto levelPtr = std::make_shared<Level>();
			game.Resources().addDrawable(id, levelPtr);
			level = levelPtr.get();
			game.Resources().setCurrentLevel(level);
		}

		if (isValidString(elem, "cel") == true
			&& isValidString(elem, "til") == true
			&& isValidString(elem, "min") == true
			&& isValidString(elem, "sol") == true
			&& isValidString(elem, "palette") == true
			&& elem.HasMember("dun") == true)
		{
			parseLevelMap(game, elem, *level);
		}

		level->Name(getStringKey(elem, "name"));

		if (elem.HasMember("followCurrentPlayer") == true)
		{
			level->FollowCurrentPlayer(getBoolVal(elem["followCurrentPlayer"]));
		}

		if (existingLevel == false)
		{
			parsePosSize(game, elem, *level);
		}

		level->resetView();
		level->updateViewport(game);

		if (elem.HasMember("onLeftClick"))
		{
			level->setAction(str2int16("leftClick"), parseAction(game, elem["onLeftClick"]));
		}
		if (elem.HasMember("onRightClick"))
		{
			level->setAction(str2int16("rightClick"), parseAction(game, elem["onRightClick"]));
		}
		if (elem.HasMember("onHoverEnter"))
		{
			level->setAction(str2int16("hoverEnter"), parseAction(game, elem["onHoverEnter"]));
		}
		if (elem.HasMember("onHoverLeave"))
		{
			level->setAction(str2int16("hoverLeave"), parseAction(game, elem["onHoverLeave"]));
		}
		if (elem.HasMember("onScrollDown"))
		{
			level->setAction(str2int16("scrollDown"), parseAction(game, elem["onScrollDown"]));
		}
		if (elem.HasMember("onScrollUp"))
		{
			level->setAction(str2int16("scrollUp"), parseAction(game, elem["onScrollUp"]));
		}
	}
}
