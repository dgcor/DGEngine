#include "ParseLevel.h"
#include "FileUtils.h"
#include "Game/LevelMap.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseTexturePack.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMap(const Value& elem, LevelMap& map, const TileSet& til, const Sol& sol)
	{
		auto pos = getVector2uKey<MapCoord>(elem, "position");
		auto file = getStringKey(elem, "file");

		if (Utils::endsWith(Utils::toLower(file), ".json") == false)
		{
			if (til.size() == 0)
			{
				return;
			}
			Dun dun(file);
			if (dun.Width() > 0 && dun.Height() > 0)
			{
				map.setArea(pos.x, pos.y, dun, til, sol);
			}
			return;
		}

		Document doc;
		if (JsonUtils::loadFile(file, doc) == false ||
			isValidArray(doc, "layers") == false ||
			doc["layers"].Empty() == true ||
			isValidArray(doc["layers"][0], "data") == false)
		{
			return;
		}

		const auto& elemDun = doc["layers"][0];

		Dun dun(getUIntKey(elemDun, "width"), getUIntKey(elemDun, "width"));

		const auto& elemData = elemDun["data"];

		for (size_t i = 0; i < elemData.Size(); i++)
		{
			dun.set(i, ((int16_t)getUIntIdx(elemData, i)) - 1);
		}

		if (dun.Width() > 0 && dun.Height() > 0)
		{
			map.setArea(pos.x, pos.y, dun, sol);
		}
	}

	void parseLevelMap(Game& game, const Value& elem, Level& level)
	{
		auto tilPath = getStringKey(elem, "til");
		auto solPath = elem["sol"].GetString();

		TileSet til(tilPath);

		Sol sol(solPath);
		if (sol.size() == 0)
		{
			return;
		}

		auto mapSize = getVector2uKey<MapCoord>(elem, "mapSize");
		LevelMap map(mapSize.x, mapSize.y);

		const auto& mapElem = elem["map"];
		if (mapElem.IsArray() == true)
		{
			for (const auto& val : mapElem)
			{
				parseMap(val, map, til, sol);
			}
		}
		else if (mapElem.IsObject() == true)
		{
			parseMap(mapElem, map, til, sol);
		}

		std::shared_ptr<TexturePack> tilesBottom;
		std::shared_ptr<TexturePack> tilesTop;

		getOrParseLevelTexturePack(game, elem,
			"texturePackBottom", "texturePackTop", tilesBottom, tilesTop);

		level.Init(std::move(map), tilesBottom, tilesTop);
	}

	void parsePosSize(const Game& game, const Value& elem, Level& level)
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
			if (isValidString(elem, "resource") == true)
			{
				game.Resources().addDrawable(elem["resource"].GetString(), id, levelPtr);
			}
			else
			{
				game.Resources().addDrawable(id, levelPtr);
			}
			level = levelPtr.get();
			game.Resources().setCurrentLevel(level);
		}

		if (isValidString(elem, "sol") == true
			&& elem.HasMember("map") == true)
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

		if (elem.HasMember("captureInputEvents"))
		{
			level->setCaptureInputEvents(getBoolVal(elem["captureInputEvents"]));
		}
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
		if (elem.HasMember("experiencePoints"))
		{
			const auto& expElem = elem["experiencePoints"];
			if (expElem.IsArray() == true)
			{
				std::vector<uint32_t> expPoints;
				expPoints.push_back(0);
				for (const auto& val : expElem)
				{
					expPoints.push_back(getUIntVal(val));
				}
				level->setExperiencePoints(expPoints);
			}
		}
	}
}
