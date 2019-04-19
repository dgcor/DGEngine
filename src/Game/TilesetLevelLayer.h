#pragma once

#include "LevelLayerInfo.h"
#include <memory>
#include "PairXY.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "SFML/Sprite2.h"
#include "SFML/View2.h"
#include "TexturePacks/TexturePack.h"
#include "TileSet.h"

class Level;
class LevelMap;

struct TilesetLevelLayer
{
	std::shared_ptr<TexturePack> tiles;
	PairInt32 visibleStart;
	PairInt32 visibleEnd;
	uint16_t layerIdx{ 0 };
	TileBlock outOfBoundsTile;

	TilesetLevelLayer() {}
	TilesetLevelLayer(const std::shared_ptr<TexturePack>& tiles_,
		uint16_t layerIdx_, const TileBlock& outOfBoundsTile_)
		: tiles(tiles_), layerIdx(layerIdx_), outOfBoundsTile(outOfBoundsTile_) {}

	void updateVisibleArea(const LevelLayerInfo& layerInfo, const LevelMap& map);

	void draw(sf::RenderTexture& levelTexture,
		const LevelLayerInfo& layerInfo, SpriteShaderCache& spriteCache,
		sf::Shader* spriteShader, const Level& level,
		bool drawLevelObjects, bool isAutomap) const;
};
