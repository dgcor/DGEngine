#pragma once

#include <cstdint>
#include <SFML/Graphics/Rect.hpp>

struct LevelLayerInfo
{
	sf::FloatRect visibleRect;
	int32_t tileWidth{ 0 };
	int32_t tileHeight{ 0 };
	int32_t blockWidth{ 0 };	// a tile is 4 blocks
	int32_t blockHeight{ 0 };
	bool visible{ false };
};
