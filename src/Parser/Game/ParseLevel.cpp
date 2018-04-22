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

	Dun getDunFromLayer(const Value& elem, int16_t indexOffset, int16_t defaultTile)
	{
		Dun dun(getUIntKey(elem, "width"), getUIntKey(elem, "height"), defaultTile);

		if (dun.Width() == 0 ||
			dun.Height() == 0 ||
			isValidArray(elem, "data") == false)
		{
			return dun;
		}
		const auto& elemData = elem["data"];
		for (size_t i = 0; i < elemData.Size(); i++)
		{
			dun.set(i, ((int16_t)getUIntIdx(elemData, i) + indexOffset));
		}
		return dun;
	}

	void parseTiledMap(const Value& elem, LevelMap& map,
		const std::string& file, int16_t defaultTile, bool resizeToFit)
	{
		Document doc;
		if (JsonUtils::loadFile(file, doc) == false ||
			isValidArray(doc, "layers") == false ||
			doc["layers"].Empty() == true)
		{
			return;
		}

		auto backName = getStringKey(elem, "back");
		auto frontName = getStringKey(elem, "front");
		auto solName = getStringKey(elem, "sol");
		auto pos = getVector2iKey<MapCoord>(elem, "position");
		bool wasResized = false;

		for (const auto& elemLayer : doc["layers"])
		{
			Dun dun = getDunFromLayer(elemLayer,
				(int16_t)getIntKey(elem, "indexOffset"),
				defaultTile);

			if (dun.Width() > 0 && dun.Height() > 0)
			{
				auto pos2 = pos;
				pos2.x += getIntKey(elemLayer, "x");
				pos2.y += getIntKey(elemLayer, "y");

				if (resizeToFit == true && wasResized == false)
				{
					map.resize((Coord)(pos2.x + dun.Width()), (Coord)(pos2.y + dun.Height()));
					wasResized = true;
				}
				if (doc["layers"].Size() == 1)
				{
					map.setSimpleArea(pos2.x, pos2.y, dun);
					continue;
				}
				auto name = getStringKey(elemLayer, "name");
				if (backName == name)
				{
					map.setSimpleArea(pos2.x, pos2.y, 0, dun);
				}
				if (frontName == name)
				{
					map.setSimpleArea(pos2.x, pos2.y, 1, dun);
				}
				if (solName == name)
				{
					map.setSimpleArea(pos2.x, pos2.y, 2, dun);
				}
			}
		}
	}

	void parseMap(const Value& elem, LevelMap& map, int16_t defaultTile, bool resizeToFit)
	{
		auto file = getStringKey(elem, "file");

		if (Utils::endsWith(Utils::toLower(file), ".json") == true)
		{
			parseTiledMap(elem, map, file, defaultTile, resizeToFit);
			return;
		}
		defaultTile = (int16_t)getIntKey(elem, "defaultTile", defaultTile);
		Dun dun(file, defaultTile);
		if (dun.Width() > 0 && dun.Height() > 0)
		{
			auto pos = getVector2uKey<MapCoord>(elem, "position");
			if (resizeToFit == true)
			{
				map.resize((Coord)(pos.x + (dun.Width() * 2)), (Coord)(pos.y + (dun.Height() * 2)));
			}
			map.setTileSetArea(pos.x, pos.y, dun);
		}
	}

	void parseLevelMap(Game& game, const Value& elem, Level& level)
	{
		auto til = getStringKey(elem, "til");
		auto sol = getStringKey(elem, "sol");
		auto mapSize = getVector2uKey<MapCoord>(elem, "mapSize", MapCoord(96, 96));
		auto defaultTile = (int16_t)getIntKey(elem, "defaultTile", -1);
		LevelMap map(til, sol, mapSize.x, mapSize.y, defaultTile);

		if (elem.HasMember("outOfBoundsTileBottom") == true)
		{
			map.setOutOfBoundsTileBack((int16_t)getIntVal(elem["outOfBoundsTileBottom"], -1));
		}
		if (elem.HasMember("outOfBoundsTileTop") == true)
		{
			map.setOutOfBoundsTileFront((int16_t)getIntVal(elem["outOfBoundsTileTop"], -1));
		}
		if (elem.HasMember("map") == true)
		{
			const auto& mapElem = elem["map"];
			if (mapElem.IsArray() == true)
			{
				for (const auto& val : mapElem)
				{
					parseMap(val, map, defaultTile, false);
				}
			}
			else if (mapElem.IsObject() == true)
			{
				bool resizeToFit = elem.HasMember("mapSize") == false;
				parseMap(mapElem, map, defaultTile, resizeToFit);
			}
		}

		std::shared_ptr<TexturePack> tilesBottom;
		std::shared_ptr<TexturePack> tilesTop;
		std::pair<uint32_t, uint32_t> tileSize;

		getOrParseLevelTexturePack(game, elem,
			"texturePackBottom", "texturePackTop", tilesBottom, tilesTop, tileSize);

		level.Init(std::move(map), tilesBottom, tilesTop, tileSize.first, tileSize.second);
	}

	void parsePosSize(const Game& game, const Value& elem, Level& level)
	{
		auto anchor = getAnchorKey(elem, "anchor");
		level.setAnchor(anchor);
		auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.WindowTexSizef());
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
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
			level->Id(id);
		}

		parseLevelMap(game, elem, *level);

		level->Name(getStringKey(elem, "name"));
		level->Path(getStringKey(elem, "path"));

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
