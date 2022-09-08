#pragma once

#include <memory>
#include "Resources/TexturePack.h"
#include "Resources/TileBlock.h"
#include <SFML/Graphics/Shader.hpp>
#include "SFML/Sprite2.h"
#include "Utils/PairXY.h"

class Level;
class LevelMap;
class LevelSurface;

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

	void updateVisibleArea(const LevelSurface& surface, const LevelMap& map);

	void draw(const LevelSurface& surface,
		SpriteShaderCache& spriteCache, GameShader* spriteShader,
		const Level& level, bool drawLevelObjects, bool isAutomap) const;
};
