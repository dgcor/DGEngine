#pragma once

#include <cstdint>
#include "Json/JsonParser.h"
#include <vector>

class Game;
struct LevelLayer;
class LevelMap;

namespace Parser
{
	uint16_t getLayerIndex(const rapidjson::Value& elem);

	void parseLevelLayer(Game& game, const LevelMap& map, const rapidjson::Value& elem,
		std::vector<LevelLayer>& levelLayers, int32_t& indexToDrawObjects);

	void parseMapLayers(LevelMap& map, const rapidjson::Value* queryDoc,
		const rapidjson::Value& elem, const PairInt32& mapPos, int32_t defaultTile, bool resizeToFit);
}
