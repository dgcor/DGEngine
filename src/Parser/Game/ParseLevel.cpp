#include "ParseLevel.h"
#include "FileUtils.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/LevelHelper.h"
#include "Game/LevelMap.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseImageContainer.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	uint16_t getLayerIndex(const Value& elem)
	{
		if (isValidString(elem, "index") == true)
		{
			auto str = getStringViewVal(elem["index"]);
			if (str == "sol")
			{
				return LevelCell::SolLayer;
			}
			return LevelCell::NumberOfLayers;
		}
		return (uint16_t)getUIntKey(elem, "index", LevelCell::NumberOfLayers);
	}

	void parseLevelLayer(Game& game, const Value& elem,
		const LevelMap& map, std::vector<LevelLayer>& levelLayers,
		int32_t& indexToDrawObjects)
	{
		bool automap = getBoolKey(elem, "automap");
		auto viewportOffset = getFloatRectKey(elem, "offset");
		if (elem.HasMember("texturePack") == true)
		{
			auto index = getLayerIndex(elem);
			if (index >= LevelCell::NumberOfLayers)
			{
				return;
			}
			auto texturePack = game.Resources().getTexturePack(getStringVal(elem["texturePack"]));
			if (texturePack == nullptr)
			{
				return;
			}
			if (getBoolKey(elem, "drawObjects") == true)
			{
				indexToDrawObjects = levelLayers.size();
			}
			TilesetLevelLayer layer(
				texturePack,
				index,
				map.getTileBlock((int16_t)getIntKey(elem, "outOfBoundsTile", -1))
			);
			levelLayers.push_back(LevelLayer(layer, viewportOffset, automap));
		}
		else if (elem.HasMember("color") == true)
		{
			ColorLevelLayer layer(getColorVal(elem["color"], sf::Color::Transparent));
			levelLayers.push_back(LevelLayer(layer, viewportOffset, automap));
		}
		else if (elem.HasMember("texture") == true)
		{
			TextureLevelLayer layer(
				game.Resources().getTexture(getStringVal(elem["texture"]))
			);
			layer.textureRect = getIntRectKey(elem, "textureRect");
			layer.parallaxSpeed = getFloatKey(elem, "parallaxSpeed");
			layer.parallaxFixedSpeed = getVector2fKey<sf::Vector2f>(elem, "parallaxFixedSpeed");
			layer.parallaxElapsedTime = getTimeKey(elem, "parallaxUpdate");
			levelLayers.push_back(LevelLayer(layer, viewportOffset, automap));
		}
	}

	bool getOrParseLevelTexturePack(Game& game, const Value& elem,
		const LevelMap& map, std::vector<LevelLayer>& levelLayers,
		std::pair<int32_t, int32_t>& tileSize, int32_t& indexToDrawObjects)
	{
		bool success = false;
		indexToDrawObjects = -1;

		if (isValidString(elem, "min") == true)
		{
			// l4.min and town.min contain 16 blocks, all others 10.
			auto minBlocks = getUIntKey(elem, "minBlocks", 10);
			if (minBlocks != 10 && minBlocks != 16)
			{
				minBlocks = 10;
			}
			Min min(getStringViewVal(elem["min"]), minBlocks);
			if (min.size() == 0)
			{
				return false;
			}

			auto pal = game.Resources().getPalette(elem["palette"].GetString());
			if (pal == nullptr)
			{
				return false;
			}

			std::shared_ptr<ImageContainer> imgCont;
			getOrParseImageContainer(game, elem, "imageContainer", imgCont);
			if (imgCont == nullptr)
			{
				return false;
			}

			bool useIndexedImages = game.Shaders().hasSpriteShader();
			CachedImagePack imgPack(imgCont.get(), pal, useIndexedImages);

			auto defaultTile = map.getTileBlock((int16_t)getIntKey(elem, "outOfBoundsTile", -1));

			TilesetLevelLayer layer1(
				LevelHelper::loadTilesetSprite(imgPack, min, false, false, true),
				0,
				defaultTile
			);
			levelLayers.push_back({ layer1, {}, false });

			TilesetLevelLayer layer2(
				LevelHelper::loadTilesetSprite(imgPack, min, true, true, true),
				0,
				defaultTile
			);
			levelLayers.push_back({ layer2, {}, false });

			tileSize.first = 64;
			tileSize.second = 32;
			success = true;
		}
		else
		{
			tileSize = getVector2uKey<std::pair<int32_t, int32_t>>(
				elem, "tileSize", std::make_pair(64, 32));
		}

		if (elem.HasMember("layers") == true)
		{
			const auto& layersElem = elem["layers"];
			if (layersElem.IsArray() == true)
			{
				for (const auto& val : layersElem)
				{
					parseLevelLayer(game, val, map, levelLayers, indexToDrawObjects);
				}
			}
			else if (layersElem.IsObject() == true)
			{
				parseLevelLayer(game, layersElem, map, levelLayers, indexToDrawObjects);
			}
			success = true;
		}
		return success;
	}

	Dun getDunFromLayer(const Value* queryDoc, const Value& elem,
		int16_t indexOffset, int16_t defaultTile)
	{
		auto width = getUIntVal(getQueryKey(queryDoc, elem, "width"));
		auto height = getUIntVal(getQueryKey(queryDoc, elem, "height"));
		Dun dun(width, height, defaultTile);

		auto& elemData = getQueryKey(queryDoc, elem, "data");

		if (dun.Width() == 0 ||
			dun.Height() == 0 ||
			elemData.IsArray() == false ||
			elemData.Size() == 0)
		{
			return dun;
		}
		for (size_t i = 0; i < elemData.Size(); i++)
		{
			dun.set(i, ((int16_t)getIntIdx(elemData, i) + indexOffset));
		}
		return dun;
	}

	void parseMapLayers(const Value* queryDoc, const Value& elem,
		LevelMap& map, const PairInt32& mapPos, int16_t defaultTile,
		bool resizeToFit)
	{
		if (isValidArray(elem, "layers") == false)
		{
			return;
		}
		auto indexOffset = (int16_t)getIntKey(elem, "indexOffset");
		auto pos = getVector2iKey<PairInt32>(elem, "position");
		pos.x += mapPos.x;
		pos.y += mapPos.y;
		bool wasResized = false;

		const auto& elemLayers = elem["layers"];
		for (const auto& elemLayer : elemLayers)
		{
			Dun dun = getDunFromLayer(queryDoc, elemLayer, indexOffset, defaultTile);

			if (dun.Width() > 0 && dun.Height() > 0)
			{
				auto pos2 = getVector2iVal<PairInt32>(
					getQueryKey(queryDoc, elemLayer, "position")
				);
				pos2.x += pos.x;
				pos2.y += pos.y;

				if (resizeToFit == true && wasResized == false)
				{
					map.resize((int32_t)(pos2.x + dun.Width()), (int32_t)(pos2.y + dun.Height()));
					wasResized = true;
				}
				if (elemLayers.Size() == 1)
				{
					map.setSimpleAreaUseSol(0, pos2.x, pos2.y, dun);
					continue;
				}
				auto index = getLayerIndex(elemLayer);
				if (index < LevelCell::NumberOfLayers)
				{
					auto normalize = getBoolKey(elemLayer, "normalize");
					map.setSimpleArea(index, pos2.x, pos2.y, dun, normalize);
				}
			}
		}
	}

	void parseMapObj(const Value* queryDoc, const Value& elem, LevelMap& map,
		const PairInt32& mapPos, int16_t defaultTile, bool resizeToFit)
	{
		std::string file;

		if (elem.HasMember("file") == true)
		{
			file = getStringVal(elem["file"]);

			if (elem.HasMember("random") == true)
			{
				auto rndMax = getUIntVal(elem["random"]);
				if (rndMax > 0)
				{
					auto rnd = Utils::Random::get(rndMax);
					Utils::replaceStringInPlace(file, "!random!", std::to_string(rnd));
				}
			}
		}

		bool hasJsonFile = false;
		Document mapDoc;

		if (Utils::endsWith(Utils::toLower(file), ".json") == true)
		{
			if (JsonUtils::loadFile(file, mapDoc) == true)
			{
				queryDoc = &mapDoc;
				hasJsonFile = true;
			}
		}

		defaultTile = (int16_t)getIntKey(elem, "defaultTile", defaultTile);

		if (elem.HasMember("layers") == true)
		{
			parseMapLayers(queryDoc, elem, map, mapPos, defaultTile, resizeToFit);
		}
		if (hasJsonFile == false)
		{
			Dun dun(file, defaultTile);
			if (dun.Width() > 0 && dun.Height() > 0)
			{
				auto pos = getVector2uKey<PairInt32>(elem, "position");
				pos.x += mapPos.x;
				pos.y += mapPos.y;
				if (resizeToFit == true)
				{
					map.resize(
						(int32_t)(pos.x + (dun.Width() * 2)),
						(int32_t)(pos.y + (dun.Height() * 2))
					);
				}
				map.setTileSetAreaUseSol(pos.x, pos.y, dun);
			}
		}
	}

	void parseMap(const Value* queryDoc, const Value& elem, LevelMap& map,
		const PairInt32& mapPos, int16_t defaultTile,
		bool resizeToFit, int recursionLevel)
	{
		if (elem.HasMember("map") == false ||
			recursionLevel < 0 || recursionLevel > 255)
		{
			return;
		}
		PairInt32 currentMapPos = mapPos;
		if (recursionLevel > 0 &&
			elem.HasMember("position") == true)
		{
			auto pos = getVector2uVal<PairInt32>(elem["position"]);
			currentMapPos.x += pos.x;
			currentMapPos.y += pos.y;
		}
		const auto& mapElem = elem["map"];
		if (mapElem.IsArray() == true)
		{
			for (const auto& val : mapElem)
			{
				parseMapObj(queryDoc, val, map, currentMapPos, defaultTile, resizeToFit);
			}
		}
		else if (mapElem.IsObject() == true)
		{
			parseMapObj(queryDoc, mapElem, map, currentMapPos, defaultTile, resizeToFit);
			if (mapElem.HasMember("map") == true)
			{
				parseMap(queryDoc, mapElem, map, currentMapPos,
					defaultTile, false, recursionLevel + 1);
			}
		}
		else if (mapElem.IsString() == true)
		{
			Document doc;
			if (JsonUtils::loadFile(getStringViewVal(mapElem), doc) == true)
			{
				parseMap(queryDoc, doc, map, currentMapPos,
					defaultTile, false, recursionLevel + 1);
			}
		}
	}

	void parseMap(const Value* queryDoc, const Value& elem,
		LevelMap& map, int16_t defaultTile)
	{
		bool resizeToFit = getBoolKey(elem, "resizeToFit", true);
		if (getBoolKey(elem, "clear", resizeToFit) == true)
		{
			map.clear();
		}
		parseMap(queryDoc, elem, map, {}, defaultTile, resizeToFit, 0);
	}

	void parseLevelMap(Game& game, const Value* queryDoc, const Value& elem, Level& level)
	{
		auto mapPtr = &level.Map();
		auto defaultTile = (int16_t)getIntVal(getQueryKey(queryDoc, elem, "defaultTile"), -1);
		PairInt32 mapSize = getVector2uVal<PairInt32>(
			getQueryKey(queryDoc, elem, "mapSize"),
			mapPtr->MapSizei()
		);

		auto& elemTil = getQueryKey(queryDoc, elem, "til");
		auto& elemSol = getQueryKey(queryDoc, elem, "sol");

		if (isValidString(elemTil) == true &&
			isValidString(elemSol) == true)
		{
			auto til = getStringViewVal(elemTil);
			auto sol = getStringViewVal(elemSol);
			*mapPtr = LevelMap(til, sol, mapSize.x, mapSize.y, defaultTile);
		}
		else
		{
			if (mapSize != mapPtr->MapSizei())
			{
				mapPtr->resize(mapSize.x, mapSize.y);
			}
		}

		if (elem.HasMember("map") == true)
		{
			parseMap(queryDoc, elem, *mapPtr, defaultTile);
		}
		if (elem.HasMember("lightMap") == true)
		{
			mapPtr->loadLightMap(getStringViewVal(getQueryKey(queryDoc, elem, "lightMap")));
		}
		if (elem.HasMember("defaultLight") == true)
		{
			mapPtr->setDefaultLightSource(getLightSourceVal(
				getQueryKey(queryDoc, elem, "defaultLight"),
				{ 0, 255, 10, LightEasing::Linear }
			));
		}

		std::vector<LevelLayer> levelLayers;
		std::pair<int32_t, int32_t> tileSize;
		int32_t indexToDrawObjects;

		if (getOrParseLevelTexturePack(game, elem, *mapPtr,
			levelLayers, tileSize, indexToDrawObjects) == true)
		{
			level.Init(game, std::move(*mapPtr), levelLayers,
				tileSize.first, tileSize.second, indexToDrawObjects);
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
				auto index = getLayerIndex(elem);
				auto defaultTile = level.Map().getTileBlock((int16_t)getIntKey(elem, "outOfBoundsTile", -1));
				auto automapTileSize = getVector2uKey<std::pair<uint32_t, uint32_t>>(
					elem, "automapTileSize", std::make_pair(64u, 32u));

				level.setAutomap(
					{ automap, index, defaultTile },
					automapTileSize.first,
					automapTileSize.second,
					getFloatRectKey(elem, "automapOffset")
				);
			}
		}
		if (elem.HasMember("automapRelativeCoords") == true)
		{
			level.setAutomapRelativeCoords(getBoolVal(elem["automapRelativeCoords"]));
		}
		if (elem.HasMember("automapAnchor") == true)
		{
			level.setAutomapAnchor(getAnchorVal(elem["automapAnchor"]));
		}
		if (elem.HasMember("automapSize") == true)
		{
			auto size = getVector2fVal<sf::Vector2f>(elem["automapSize"], { 100.f, 100.f });
			level.setAutomapSize(size);
		}
		if (elem.HasMember("automapPosition") == true)
		{
			auto anchor = level.getAutomapAnchor();
			auto size = level.getAutomapSize();
			auto pos = getVector2fVal<sf::Vector2f>(elem["automapPosition"]);
			if (level.getAutomapRelativeCoords() == false &&
				game.RefSize() != game.DrawRegionSize())
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			}
			level.setAutomapPosition(pos);
		}
		if (elem.HasMember("automapPlayerDirectionIndex") == true)
		{
			auto index = (int16_t)getIntVal(elem["automapPlayerDirectionIndex"], -1);
			level.setAutomapPlayerDirectionBaseIndex(index);
		}
		if (elem.HasMember("showAutomap") == true)
		{
			level.ShowAutomap(getBoolVal(elem["showAutomap"]));
		}
	}

	void parsePosSize(const Game& game, const Value* queryObj,
		const Value& elem, Level& level)
	{
		auto anchor = getAnchorKey(elem, "anchor");
		level.setAnchor(anchor);
		auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.DrawRegionSizef());
		auto pos = getPositionKey(elem, "position", size, game.RefSize());
		if (getBoolKey(elem, "relativeCoords", true) == true &&
			game.RefSize() != game.DrawRegionSize())
		{
			GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
		}
		level.Position(pos);
		level.Size(size);
		level.Zoom(((float)getIntVal(getQueryKey(queryObj, elem, "zoom"), 100)) / 100.f);
		level.Visible(getBoolVal(getQueryKey(queryObj, elem, "visible"), true));
	}

	void parseLevel(Game& game, const Value& elem)
	{
		Document queryDoc;
		const Value* queryObj = nullptr;
		if (isValidString(elem, "load") == true)
		{
			if (JsonUtils::loadFile(getStringViewVal(elem["load"]), queryDoc) == true)
			{
				queryObj = &queryDoc;
			}
		}

		auto id = getStringVal(getQueryKey(queryObj, elem, "id"));
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
			parsePosSize(game, queryObj, elem, *level);
		}

		parseLevelMap(game, queryObj, elem, *level);
		parseLevelAutomap(game, elem, *level);

		if (elem.HasMember("name") == true)
		{
			level->Name(getStringVal(getQueryVal(queryObj, elem["name"])));
		}
		if (elem.HasMember("path") == true)
		{
			level->Path(getStringVal(getQueryVal(queryObj, elem["path"])));
		}
		if (elem.HasMember("followCurrentPlayer") == true)
		{
			auto followCurrentPlayer = getBoolVal(getQueryVal(queryObj, elem["followCurrentPlayer"]));
			level->FollowCurrentPlayer(followCurrentPlayer);
		}
		if (elem.HasMember("smoothMovement") == true)
		{
			auto smoothMovement = getBoolVal(getQueryVal(queryObj, elem["smoothMovement"]));
			level->setSmoothMovement(smoothMovement);
		}

		level->resetView();
		level->updateViewport(game);

		if (elem.HasMember("enableHover"))
		{
			bool enableHover = getBoolVal(getQueryVal(queryObj, elem["enableHover"]), true);
			level->EnableHover(enableHover);
		}
		if (elem.HasMember("captureInputEvents"))
		{
			auto captureInputEvents = getInputEventVal(getQueryVal(queryObj, elem["captureInputEvents"]));
			level->setCaptureInputEvents(captureInputEvents);
		}
		if (elem.HasMember("onLeftClick"))
		{
			level->setAction(
				str2int16("leftClick"),
				parseAction(game, getQueryVal(queryObj, elem["onLeftClick"]))
			);
		}
		if (elem.HasMember("onRightClick"))
		{
			level->setAction(
				str2int16("rightClick"),
				parseAction(game, getQueryVal(queryObj, elem["onRightClick"]))
			);
		}
		if (elem.HasMember("onHoverEnter"))
		{
			level->setAction(
				str2int16("hoverEnter"),
				parseAction(game, getQueryVal(queryObj, elem["onHoverEnter"]))
			);
		}
		if (elem.HasMember("onHoverLeave"))
		{
			level->setAction(
				str2int16("hoverLeave"),
				parseAction(game, getQueryVal(queryObj, elem["onHoverLeave"]))
			);
		}
		if (elem.HasMember("onScrollDown"))
		{
			level->setAction(
				str2int16("scrollDown"),
				parseAction(game, getQueryVal(queryObj, elem["onScrollDown"]))
			);
		}
		if (elem.HasMember("onScrollUp"))
		{
			level->setAction(
				str2int16("scrollUp"),
				parseAction(game, getQueryVal(queryObj, elem["onScrollUp"]))
			);
		}
		if (elem.HasMember("experiencePoints"))
		{
			const auto& expElem = getQueryVal(queryObj, elem["experiencePoints"]);
			if (expElem.IsArray() == true)
			{
				std::vector<uint32_t> expPoints;
				expPoints.push_back(0);
				for (const auto& val : expElem)
				{
					expPoints.push_back(getUIntVal(getQueryVal(queryObj, val)));
				}
				level->setExperiencePoints(expPoints);
			}
		}
	}
}
