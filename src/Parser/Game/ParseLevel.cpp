#include "ParseLevel.h"
#include "FileUtils.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/LevelMap.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseImageContainer.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/Utils.h"
#ifndef NO_DIABLO_FORMAT_SUPPORT
#include "DS1.h"
#endif

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	uint16_t getLayerIndex(const Value& elem)
	{
		if (isValidString(elem, "index") == true)
		{
			auto str = getStringViewVal(elem["index"sv]);
			if (str == "flags")
			{
				return LevelCell::FlagsLayer;
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
		if (elem.HasMember("texturePack"sv) == true)
		{
			auto index = getLayerIndex(elem);
			if (index >= LevelCell::NumberOfLayers)
			{
				return;
			}
			auto texturePack = game.Resources().getTexturePack(getStringViewVal(elem["texturePack"sv]));
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
		else if (elem.HasMember("color"sv) == true)
		{
			ColorLevelLayer layer(getColorVal(elem["color"sv], sf::Color::Transparent));
			levelLayers.push_back(LevelLayer(layer, viewportOffset, automap));
		}
		else if (elem.HasMember("texture"sv) == true)
		{
			TextureLevelLayer layer(
				game.Resources().getTexture(getStringViewVal(elem["texture"sv]))
			);
			layer.textureRect = getIntRectKey(elem, "textureRect");
			layer.parallaxSpeed = getFloatKey(elem, "parallaxSpeed");
			layer.parallaxFixedSpeed = getVector2fKey<sf::Vector2f>(elem, "parallaxFixedSpeed");
			layer.parallaxElapsedTime = getTimeKey(elem, "parallaxUpdate");
			levelLayers.push_back(LevelLayer(layer, viewportOffset, automap));
		}
	}

	Vector2D<int32_t> getDunFromLayer(const Value* queryDoc, const Value& elem,
		int32_t indexOffset, int32_t defaultTile)
	{
		auto width = getUIntVal(getQueryKey(queryDoc, elem, "width"));
		auto height = getUIntVal(getQueryKey(queryDoc, elem, "height"));
		Vector2D<int32_t> dun(width, height, defaultTile);

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
			dun.set(i, (getIntIdx(elemData, i) + indexOffset));
		}
		return dun;
	}

	void parseMapLayers(const Value* queryDoc, const Value& elem,
		LevelMap& map, const PairInt32& mapPos, int32_t defaultTile,
		bool resizeToFit)
	{
		if (isValidArray(elem, "layers") == false)
		{
			return;
		}
		auto indexOffset = (int32_t)getIntKey(elem, "indexOffset");
		auto pos = getVector2iKey<PairInt32>(elem, "position");
		pos.x += mapPos.x;
		pos.y += mapPos.y;
		bool wasResized = false;

		const auto& elemLayers = elem["layers"sv];
		for (const auto& elemLayer : elemLayers)
		{
			auto dun = getDunFromLayer(queryDoc, elemLayer, indexOffset, defaultTile);

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
					map.setSimpleAreaUseFlags(0, pos2.x, pos2.y, dun);
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
		const PairInt32& mapPos, int32_t defaultTile, bool resizeToFit)
	{
		std::string file;

		if (elem.HasMember("file"sv) == true)
		{
			file = getStringVal(elem["file"sv]);

			if (elem.HasMember("random"sv) == true)
			{
				auto rndMax = getUIntVal(elem["random"sv]);
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
#ifndef NO_DIABLO_FORMAT_SUPPORT
		else if (Utils::endsWith(Utils::toLower(file), ".ds1") == true)
		{
			auto ds1 = DS1::Decoder(file);
			map.setD2Area(0, 0, ds1);
			return;
		}
#endif

		defaultTile = getIntKey(elem, "defaultTile", defaultTile);

		if (elem.HasMember("layers"sv) == true)
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
				map.setTileSetAreaUseFlags(pos.x, pos.y, dun);
			}
		}
	}

	void parseMap(const Value* queryDoc, const Value& elem, LevelMap& map,
		const PairInt32& mapPos, int32_t defaultTile,
		bool resizeToFit, int recursionLevel)
	{
		if (elem.HasMember("map"sv) == false ||
			recursionLevel < 0 || recursionLevel > 255)
		{
			return;
		}
		PairInt32 currentMapPos = mapPos;
		if (recursionLevel > 0 &&
			elem.HasMember("position"sv) == true)
		{
			auto pos = getVector2uVal<PairInt32>(elem["position"sv]);
			currentMapPos.x += pos.x;
			currentMapPos.y += pos.y;
		}
		const auto& mapElem = elem["map"sv];
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
			if (mapElem.HasMember("map"sv) == true)
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
		LevelMap& map, int32_t defaultTile)
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
		auto defaultTile = getIntVal(getQueryKey(queryDoc, elem, "defaultTile"), -1);
		PairInt32 mapSize = getVector2uVal<PairInt32>(
			getQueryKey(queryDoc, elem, "mapSize"),
			mapPtr->MapSizei()
		);

		auto& elemTil = getQueryKey(queryDoc, elem, "til");
		auto& elemFlags = getQueryKey(queryDoc, elem, "flags");

		if (isValidString(elemTil) == true &&
			isValidString(elemFlags) == true)
		{
			auto til = getStringViewVal(elemTil);
			auto flags = getStringViewVal(elemFlags);
			*mapPtr = LevelMap(til, flags, mapSize.x, mapSize.y, defaultTile);
		}
		else if (isValidString(elemFlags) == true &&
			isValidId(elemFlags.GetStringView()) == true)
		{
			auto flags = elemFlags.GetStringView();
			auto texPack = game.Resources().getTexturePack(flags);
			if (texPack != nullptr)
			{
				auto levelFlags = std::dynamic_pointer_cast<LevelFlags>(texPack);
				*mapPtr = LevelMap(levelFlags, mapSize.x, mapSize.y, defaultTile);
			}
		}
		else
		{
			if (elem.HasMember("flags"sv) == true &&
				elem["flags"sv].IsNull() == true)
			{
				mapPtr->setFlags({});
			}
			if (mapSize != mapPtr->MapSizei())
			{
				mapPtr->resize(mapSize.x, mapSize.y);
			}
		}

		if (elem.HasMember("map"sv) == true)
		{
			parseMap(queryDoc, elem, *mapPtr, defaultTile);
		}
		if (elem.HasMember("lightMap"sv) == true)
		{
			mapPtr->loadLightMap(getStringViewVal(getQueryKey(queryDoc, elem, "lightMap")));
		}
		if (elem.HasMember("defaultLight"sv) == true)
		{
			mapPtr->setDefaultLightSource(getLightSourceVal(
				getQueryKey(queryDoc, elem, "defaultLight"),
				{ 255, 10 }
			));
		}

		std::vector<LevelLayer> levelLayers;
		auto tileSize = std::make_pair(level.TileWidth(), level.TileHeight());
		if (tileSize.first == 0 && tileSize.second == 0)
		{
			tileSize = std::make_pair(64, 32);
		}
		tileSize = getVector2uKey(elem, "tileSize", tileSize);
		auto subTiles = getUIntKey(elem, "subTiles", level.SubTiles());
		int32_t indexToDrawObjects = -1;

		bool parsedLayers = false;
		if (elem.HasMember("layers"sv) == true)
		{
			const auto& layersElem = elem["layers"sv];
			if (layersElem.IsArray() == true)
			{
				for (const auto& val : layersElem)
				{
					parseLevelLayer(game, val, *mapPtr, levelLayers, indexToDrawObjects);
				}
				parsedLayers = true;
			}
			else if (layersElem.IsObject() == true)
			{
				parseLevelLayer(game, layersElem, *mapPtr, levelLayers, indexToDrawObjects);
				parsedLayers = true;
			}
		}
		if (parsedLayers == true)
		{
			level.Init(game, std::move(*mapPtr), levelLayers,
				tileSize.first, tileSize.second, subTiles, indexToDrawObjects);
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
			auto automap = game.Resources().getTexturePack(elem["automap"sv].GetStringView());
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
		if (elem.HasMember("automapRelativeCoords"sv) == true)
		{
			level.setAutomapRelativeCoords(getBoolVal(elem["automapRelativeCoords"sv]));
		}
		if (elem.HasMember("automapAnchor"sv) == true)
		{
			level.setAutomapAnchor(getAnchorVal(elem["automapAnchor"sv]));
		}
		if (elem.HasMember("automapSize"sv) == true)
		{
			auto size = getVector2fVal<sf::Vector2f>(elem["automapSize"sv], { 100.f, 100.f });
			level.setAutomapSize(size);
		}
		if (elem.HasMember("automapPosition"sv) == true)
		{
			auto anchor = level.getAutomapAnchor();
			auto size = level.getAutomapSize();
			auto pos = getVector2fVal<sf::Vector2f>(elem["automapPosition"sv]);
			if (level.getAutomapRelativeCoords() == false &&
				game.RefSize() != game.DrawRegionSize())
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			}
			level.setAutomapPosition(pos);
		}
		if (elem.HasMember("automapPlayerDirectionIndex"sv) == true)
		{
			auto index = (int16_t)getIntVal(elem["automapPlayerDirectionIndex"sv], -1);
			level.setAutomapPlayerDirectionBaseIndex(index);
		}
		if (elem.HasMember("showAutomap"sv) == true)
		{
			level.ShowAutomap(getBoolVal(elem["showAutomap"sv]));
		}
	}

	void parseLevel(Game& game, const Value& elem)
	{
		Document queryDoc;
		const Value* queryObj = nullptr;
		if (isValidString(elem, "load") == true)
		{
			if (JsonUtils::loadFile(elem["load"sv].GetStringView(), queryDoc) == true)
			{
				queryObj = &queryDoc;
			}
		}

		auto id = getStringViewVal(getQueryKey(queryObj, elem, "id"));
		Level* level = game.Resources().getLevel(id);
		if (level == nullptr)
		{
			if (isValidId(id) == false)
			{
				return;
			}
			auto levelPtr = std::make_shared<Level>();
			game.Resources().addDrawable(id, levelPtr, true, getStringViewKey(elem, "resource"));
			level = levelPtr.get();
			game.Resources().setCurrentLevel(level);
			level->Id(id);
			level->setShader(game.Shaders().Level);

			auto anchor = getAnchorKey(elem, "anchor");
			level->setAnchor(anchor);
			auto size = getVector2fKey<sf::Vector2f>(elem, "size", game.DrawRegionSizef());
			auto pos = getPositionKey(elem, "position", size, game.RefSize());
			if (getBoolKey(elem, "relativeCoords", true) == true &&
				game.RefSize() != game.DrawRegionSize())
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			}
			level->Position(pos);
			level->Size(size);
			level->Zoom(((float)getIntVal(getQueryKey(queryObj, elem, "zoom"), 100)) / 100.f);
			level->Visible(getBoolVal(getQueryKey(queryObj, elem, "visible"), true));
		}

		parseLevelMap(game, queryObj, elem, *level);
		parseLevelAutomap(game, elem, *level);

		if (elem.HasMember("name"sv) == true)
		{
			level->Name(getStringViewVal(getQueryVal(queryObj, elem["name"sv])));
		}
		if (elem.HasMember("path"sv) == true)
		{
			level->Path(getStringViewVal(getQueryVal(queryObj, elem["path"sv])));
		}
		if (elem.HasMember("followCurrentPlayer"sv) == true)
		{
			auto followCurrentPlayer = getBoolVal(getQueryVal(queryObj, elem["followCurrentPlayer"sv]));
			level->FollowCurrentPlayer(followCurrentPlayer);
		}
		if (elem.HasMember("smoothMovement"sv) == true)
		{
			auto smoothMovement = getBoolVal(getQueryVal(queryObj, elem["smoothMovement"sv]));
			level->setSmoothMovement(smoothMovement);
		}
		if (elem.HasMember("lightRadius"sv) == true)
		{
			level->LightRadius((float)getUIntVal(getQueryVal(queryObj, elem["lightRadius"sv]), 64));
		}

		level->updateView();

		if (elem.HasMember("enableHover"sv))
		{
			bool enableHover = getBoolVal(getQueryVal(queryObj, elem["enableHover"sv]), true);
			level->EnableHover(enableHover);
		}
		if (elem.HasMember("captureInputEvents"sv))
		{
			auto captureInputEvents = getInputEventTypeVal(getQueryVal(queryObj, elem["captureInputEvents"sv]));
			level->setCaptureInputEvents(captureInputEvents);
		}
		if (elem.HasMember("onLeftClick"sv))
		{
			level->setAction(
				str2int16("leftClick"),
				getActionVal(game, getQueryVal(queryObj, elem["onLeftClick"sv]))
			);
		}
		if (elem.HasMember("onRightClick"sv))
		{
			level->setAction(
				str2int16("rightClick"),
				getActionVal(game, getQueryVal(queryObj, elem["onRightClick"sv]))
			);
		}
		if (elem.HasMember("onHoverEnter"sv))
		{
			level->setAction(
				str2int16("hoverEnter"),
				getActionVal(game, getQueryVal(queryObj, elem["onHoverEnter"sv]))
			);
		}
		if (elem.HasMember("onHoverLeave"sv))
		{
			level->setAction(
				str2int16("hoverLeave"),
				getActionVal(game, getQueryVal(queryObj, elem["onHoverLeave"sv]))
			);
		}
		if (elem.HasMember("onScrollDown"sv))
		{
			level->setAction(
				str2int16("scrollDown"),
				getActionVal(game, getQueryVal(queryObj, elem["onScrollDown"sv]))
			);
		}
		if (elem.HasMember("onScrollUp"sv))
		{
			level->setAction(
				str2int16("scrollUp"),
				getActionVal(game, getQueryVal(queryObj, elem["onScrollUp"sv]))
			);
		}
		if (elem.HasMember("experiencePoints"sv))
		{
			const auto& expElem = getQueryVal(queryObj, elem["experiencePoints"sv]);
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
