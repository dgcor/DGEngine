#include "ParseLevel.h"
#include "Cel.h"
#include "FileUtils.h"
#include "Game/LevelMap.h"
#include "GameUtils.h"
#include "ParseAction.h"
#include "ParseUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using Utils::str2int;

	void parseDun(const Value& elem, LevelMap& map, const TileSet& til, const Sol& sol)
	{
		Dun dun(getString(elem, "file"));
		if (dun.Width() > 0 && dun.Height() > 0)
		{
			auto pos = getVector2u<sf::Vector2u>(elem, "position");
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
		Sol sol(solPath);

		auto mapSize = getVector2u<sf::Vector2u>(elem, "mapSize");
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
		Min min(minPath, getInt(elem, "minBlocks", 10));

		auto pal = game.Resources().getPalette(palPath);
		if (pal == nullptr)
		{
			return;
		}
		bool isCl2 = Utils::endsWith(celPath, "cl2");
		CelFile cel(celPath, isCl2, true);

		CelFrameCache celCache(cel, *pal);
		level.Init(map, min, celCache);
	}

	void parsePosSize(Game& game, const Value& elem, Level& level)
	{
		auto anchor = getAnchor(elem, "anchor");
		level.setAnchor(anchor);
		auto pos = getVector2f<sf::Vector2f>(elem, "position");
		auto size = getVector2f<sf::Vector2f>(elem, "size", game.WindowTexSizef());
		if (getBool(elem, "relativeCoords", true) == true)
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.MinSize());
			if (game.StretchToFit() == false)
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.MinSize(), game.WindowSize());
			}
		}
		level.Position(pos);
		level.Size(size);
		level.Visible(getBool(elem, "visible", true));
	}

	void parseLevelOption(Game& game, Level& level, unsigned optionHash)
	{
		switch (optionHash)
		{
		case str2int("clearPlayers"):
		{
			level.Players().clear();
			break;
		}
		default:
			break;
		}
	}

	void parseLevelOptions(Game& game, const Value& elem, Level& level)
	{
		if (elem.HasMember("options") == true)
		{
			const auto& dunElem = elem["options"];
			if (dunElem.IsArray() == true)
			{
				for (const auto& val : dunElem)
				{
					parseLevelOption(game, level, str2int(getStringChar_(val)));
				}
			}
			else if (dunElem.IsString() == true)
			{
				parseLevelOption(game, level, str2int(dunElem.GetString()));
			}
		}
	}

	void parseLevel(Game& game, const Value& elem)
	{
		auto id = getString(elem, "id");
		Level* level = game.Resources().getResource<Level>(id);
		bool existingLevel = (level != nullptr);
		if (level == nullptr)
		{
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

		if (existingLevel == false)
		{
			parsePosSize(game, elem, *level);
		}
		else
		{
			parseLevelOptions(game, elem, *level);
		}

		level->resetView();
		level->updateViewPort(game);

		if (elem.HasMember("onLeftClick"))
		{
			level->setLeftAction(parseAction(game, elem["onLeftClick"]));
		}

		if (elem.HasMember("onRightClick"))
		{
			level->setRightAction(parseAction(game, elem["onRightClick"]));
		}
	}
}
