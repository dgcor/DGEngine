#include "ParseLevelMap.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Level/LevelMap.h"
#include "Json/JsonUtils.h"
#include "ParseLevelLayer.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "Resources/DS1.h"
#endif
#include "Utils/Random.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

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
					auto rnd = Random::get(rndMax);
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
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
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
			parseMapLayers(map, queryDoc, elem, mapPos, defaultTile, resizeToFit);
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
		const PairInt32& mapPos, int32_t defaultTile, bool resizeToFit, int recursionLevel)
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

	void parseMap(const Value* queryDoc, const Value& elem, LevelMap& map, int32_t defaultTile)
	{
		bool resizeToFit = getBoolKey(elem, "resizeToFit", true);
		if (getBoolKey(elem, "clear", resizeToFit) == true)
		{
			map.clear();
		}
		parseMap(queryDoc, elem, map, {}, defaultTile, resizeToFit, 0);
	}

	void parseLevelMap(Game& game, Level& level, const Value* queryDoc, const Value& elem)
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
					parseLevelLayer(game, *mapPtr, val, levelLayers, indexToDrawObjects);
				}
				parsedLayers = true;
			}
			else if (layersElem.IsObject() == true)
			{
				parseLevelLayer(game, *mapPtr, layersElem, levelLayers, indexToDrawObjects);
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
}
