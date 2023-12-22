#pragma once

#include "SFML/Surface.h"

class LevelObject;

class LevelSurface : public Surface
{
public:
	int32_t tileWidth{ 0 };
	int32_t tileHeight{ 0 };
	int32_t blockWidth{ 0 };	// a tile is 4 blocks
	int32_t blockHeight{ 0 };
	uint32_t subTiles{ 0 };

	LevelSurface() : Surface(false, true) {}

	using Surface::draw;

	void draw(const LevelObject& obj, GameShader* spriteShader, SpriteShaderCache& cache) const;
};
