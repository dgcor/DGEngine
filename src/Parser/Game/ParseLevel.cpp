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

	void parseLayerIndexName(const Value& elem,
		std::vector<std::pair<std::string, size_t>>& layerNameIndexes)
	{
		layerNameIndexes.push_back(
			std::make_pair(getStringKey(elem, "name"), getUIntKey(elem, "index"))
		);
	}

	void parseMapLayers(const Value& elem, const Value& elemLayers,
		LevelMap& map, const MapCoord& mapPos, int16_t defaultTile,
		bool resizeToFit, bool mapToNames)
	{
		std::vector<std::pair<std::string, size_t>> layerNameIndexes;

		if (mapToNames == true)
		{
			// if the layer data comes from a different file (Tiled map),
			// get a map of names to indexes
			if (elem.HasMember("layers") == true)
			{
				const auto& namesElem = elem["layers"];
				if (namesElem.IsArray() == true)
				{
					for (const auto& val : namesElem)
					{
						parseLayerIndexName(val, layerNameIndexes);
					}
				}
				else if (namesElem.IsObject() == true)
				{
					parseLayerIndexName(namesElem, layerNameIndexes);
				}
			}
		}

		auto automapName = getStringViewKey(elem, "automap");
		auto solName = getStringViewKey(elem, "sol");
		auto pos = getVector2iKey<MapCoord>(elem, "position");
		pos.x += mapPos.x;
		pos.y += mapPos.y;
		bool wasResized = false;

		for (const auto& elemLayer : elemLayers)
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
				if (elemLayers.Size() == 1)
				{
					map.setSimpleArea(pos2.x, pos2.y, dun);
					continue;
				}

				if (mapToNames == false)
				{
					auto index = getUIntKey(elemLayer, "index");
					if (index < LevelCell::NumberOfLayers)
					{
						map.setSimpleArea(pos2.x, pos2.y, index, dun, false);
					}
				}
				else
				{
					auto name = getStringViewKey(elemLayer, "name");

					for (const auto& layerNameIndex : layerNameIndexes)
					{
						if (layerNameIndex.first == name &&
							layerNameIndex.second < LevelCell::NumberOfLayers)
						{
							map.setSimpleArea(pos2.x, pos2.y, layerNameIndex.second, dun);
						}
					}
					if (automapName == name)
					{
						map.setSimpleArea(pos2.x, pos2.y, Level::AutomapLayer, dun);
					}
					if (solName == name)
					{
						map.setSimpleArea(pos2.x, pos2.y, LevelCell::SolLayer, dun);
					}
				}
			}
		}
	}

	void parseTiledMap(const Value& elem, LevelMap& map,
		const MapCoord& mapPos, const std::string_view file,
		int16_t defaultTile, bool resizeToFit)
	{
		Document doc;
		if (JsonUtils::loadFile(file, doc) == false ||
			isValidArray(doc, "layers") == false ||
			doc["layers"].Empty() == true)
		{
			return;
		}
		parseMapLayers(elem, doc["layers"], map, mapPos, defaultTile, resizeToFit, true);
	}

	void parseMapObj(const Value& elem, LevelMap& map,
		const MapCoord& mapPos, int16_t defaultTile, bool resizeToFit)
	{
		defaultTile = (int16_t)getIntKey(elem, "defaultTile", defaultTile);

		if (elem.HasMember("file") == false)
		{
			if (elem.HasMember("layers") == true)
			{
				parseMapLayers(elem, elem["layers"], map, mapPos, defaultTile, resizeToFit, false);
			}
			return;
		}

		auto file = getStringVal(elem["file"]);

		if (elem.HasMember("random") == true)
		{
			auto rndMax = getUIntVal(elem["random"]);
			if (rndMax > 0)
			{
				auto rnd = Utils::Random::get(rndMax);
				Utils::replaceStringInPlace(file, "!random!", std::to_string(rnd));
			}
		}

		if (Utils::endsWith(Utils::toLower(file), ".json") == true)
		{
			parseTiledMap(elem, map, mapPos, file, defaultTile, resizeToFit);
			return;
		}

		Dun dun(file, defaultTile);
		if (dun.Width() > 0 && dun.Height() > 0)
		{
			auto pos = getVector2uKey<MapCoord>(elem, "position");
			pos.x += mapPos.x;
			pos.y += mapPos.y;
			if (resizeToFit == true)
			{
				map.resize((Coord)(pos.x + (dun.Width() * 2)), (Coord)(pos.y + (dun.Height() * 2)));
			}
			map.setTileSetArea(pos.x, pos.y, dun);
		}
	}

	void parseMap(const Value& elem, LevelMap& map,
		const MapCoord& mapPos, int16_t defaultTile, int recursionLevel)
	{
		if (elem.HasMember("map") == false ||
			recursionLevel < 0 || recursionLevel > 255)
		{
			return;
		}
		MapCoord currentMapPos = mapPos;
		if (recursionLevel > 0 &&
			elem.HasMember("position") == true)
		{
			auto pos = getVector2uVal<MapCoord>(elem["position"]);
			currentMapPos.x += pos.x;
			currentMapPos.y += pos.y;
		}
		const auto& mapElem = elem["map"];
		if (mapElem.IsArray() == true)
		{
			bool resizeToFit = getBoolKey(elem, "resizeToFit", false);
			for (const auto& val : mapElem)
			{
				parseMapObj(val, map, currentMapPos, defaultTile, resizeToFit);
			}
		}
		else if (mapElem.IsObject() == true)
		{
			bool resizeToFit = getBoolKey(elem, "resizeToFit", true);
			parseMapObj(mapElem, map, currentMapPos, defaultTile, resizeToFit);
			if (mapElem.HasMember("map") == true)
			{
				parseMap(mapElem, map, currentMapPos, defaultTile, recursionLevel + 1);
			}
		}
		else if (mapElem.IsString() == true)
		{
			Document doc;
			if (JsonUtils::loadFile(getStringViewVal(mapElem), doc) == true)
			{
				parseMap(doc, map, currentMapPos, defaultTile, recursionLevel + 1);
			}
		}
	}

	void parseMap(const Value& elem, LevelMap& map, int16_t defaultTile)
	{
		parseMap(elem, map, {}, defaultTile, 0);
	}

	void parseLevelMap(Game& game, const Value& elem, Level& level)
	{
		auto mapPtr = &level.Map();
		auto defaultTile = (int16_t)getIntKey(elem, "defaultTile", -1);
		MapCoord mapSize = getVector2uKey<MapCoord>(elem, "mapSize", mapPtr->MapSize());

		if (isValidString(elem, "til") == true &&
			isValidString(elem, "sol") == true)
		{
			auto til = getStringViewVal(elem["til"]);
			auto sol = getStringViewVal(elem["sol"]);
			*mapPtr = LevelMap(til, sol, mapSize.x, mapSize.y, defaultTile);
		}
		else
		{
			if (mapSize != mapPtr->MapSize())
			{
				mapPtr->resize(mapSize.x, mapSize.y, defaultTile);
			}
		}

		if (elem.HasMember("outOfBoundsTileLayer1") == true)
		{
			mapPtr->setOutOfBoundsTileIndex(0, (int16_t)getIntVal(elem["outOfBoundsTileLayer1"], -1));
		}
		if (elem.HasMember("outOfBoundsTileLayer2") == true)
		{
			mapPtr->setOutOfBoundsTileIndex(1, (int16_t)getIntVal(elem["outOfBoundsTileLayer2"], -1));
		}
		if (elem.HasMember("outOfBoundsTileLayer3") == true)
		{
			mapPtr->setOutOfBoundsTileIndex(2, (int16_t)getIntVal(elem["outOfBoundsTileLayer3"], -1));
		}
		if (elem.HasMember("outOfBoundsTileAutomap") == true)
		{
			mapPtr->setOutOfBoundsTileIndex(3, (int16_t)getIntVal(elem["outOfBoundsTileAutomap"], -1));
		}
		if (elem.HasMember("map") == true)
		{
			parseMap(elem, *mapPtr, defaultTile);
		}
		if (elem.HasMember("lightMap") == true)
		{
			mapPtr->loadLightMap(getStringViewVal(elem["lightMap"]));
		}
		if (elem.HasMember("defaultLight") == true)
		{
			mapPtr->setDefaultLightSource(
				getLightSourceVal(elem["defaultLight"], { 0, 255, 10, LightEasing::Linear })
			);
		}

		std::vector<std::shared_ptr<TexturePack>> texturePackLayers;
		std::pair<int32_t, int32_t> tileSize;

		if (getOrParseLevelTexturePack(game, elem, "layers", texturePackLayers, tileSize) == true)
		{
			level.Init(game, std::move(*mapPtr), texturePackLayers, tileSize.first, tileSize.second);
		}
		else
		{
			level.Init();
		}
	}

	void parseLevelAutomap(Game& game, const Value& elem, Level& level)
	{
		if (isValidString(elem, "automap") == true)
		{
			auto automap = game.Resources().getTexturePack(elem["automap"].GetString());
			if (automap != nullptr)
			{
				auto automapTileSize = getVector2uKey<std::pair<uint32_t, uint32_t>>(
					elem, "automapTileSize", std::make_pair(64u, 32u));

				level.setAutomap(automap, automapTileSize.first, automapTileSize.second);
			}
		}
		if (elem.HasMember("automapBackground") == true)
		{
			auto color = getColorVal(elem["automapBackground"], sf::Color::Transparent);
			level.setAutomapBackgroundColor(color);
		}
		if (elem.HasMember("automapPosition") == true)
		{
			auto pos = getVector2iVal<sf::Vector2i>(elem["automapPosition"]);
			level.setAutomapRelativePosition(pos);
		}
		if (elem.HasMember("automapSize") == true)
		{
			auto size = getVector2iVal<sf::Vector2i>(elem["automapSize"], { 100, 100 });
			level.setAutomapRelativeSize(size);
		}
		if (elem.HasMember("automapPlayerDirectionIndex") == true)
		{
			auto index = getIntVal(elem["automapPlayerDirectionIndex"], -1);
			level.setAutomapPlayerDirectionBaseIndex(index);
		}
		if (elem.HasMember("showAutomap") == true)
		{
			level.ShowAutomap(getBoolVal(elem["showAutomap"]));
		}
	}

	void parsePosSize(const Game& game, const Value& elem, Level& level)
	{
		auto anchor = getAnchorKey(elem, "anchor");
		level.setAnchor(anchor);
		auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.DrawRegionSizef());
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
		Level* level = game.Resources().getDrawable<Level>(id);
		bool existingLevel = (level != nullptr);
		if (level == nullptr)
		{
			if (isValidId(id) == false)
			{
				return;
			}
			auto levelPtr = std::make_shared<Level>();
			game.Resources().addDrawable(id, levelPtr, getStringViewKey(elem, "resource"));
			level = levelPtr.get();
			game.Resources().setCurrentLevel(level);
			level->Id(id);
		}

		if (existingLevel == false)
		{
			parsePosSize(game, elem, *level);
		}

		parseLevelMap(game, elem, *level);
		parseLevelAutomap(game, elem, *level);

		if (elem.HasMember("name") == true)
		{
			level->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("path") == true)
		{
			level->Path(getStringVal(elem["path"]));
		}
		if (elem.HasMember("followCurrentPlayer") == true)
		{
			level->FollowCurrentPlayer(getBoolVal(elem["followCurrentPlayer"]));
		}

		level->resetView();
		level->updateViewport(game);

		if (elem.HasMember("enableHover"))
		{
			level->EnableHover(getBoolKey(elem, "enableHover", true));
		}
		if (elem.HasMember("captureInputEvents"))
		{
			level->setCaptureInputEvents(getInputEventVal(elem["captureInputEvents"]));
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
