#include "ParseLevel.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Parser/Utils/ParseUtils.h"

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

	void parseLevelLayer(Game& game, const LevelMap& map, const Value& elem,
		std::vector<LevelLayer>& levelLayers, int32_t& indexToDrawObjects)
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
				indexToDrawObjects = (int32_t)levelLayers.size();
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

	Vector2D<int32_t> getDunFromLayer(const Value* queryDoc, const Value& elem, int32_t indexOffset, int32_t defaultTile)
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
		for (SizeType i = 0; i < elemData.Size(); i++)
		{
			dun.set(i, (getIntIdx(elemData, i) + indexOffset));
		}
		return dun;
	}

	void parseMapLayers(LevelMap& map, const Value* queryDoc, const Value& elem,
		const PairInt32& mapPos, int32_t defaultTile, bool resizeToFit)
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
}
