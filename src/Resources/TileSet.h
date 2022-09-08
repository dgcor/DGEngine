#pragma once

#include <string_view>
#include "TileBlock.h"
#include <vector>

class TileSet
{
private:
	std::vector<TileBlock> tiles;

public:
	TileSet() {}
	TileSet(const std::string_view fileName);

	const TileBlock& operator[] (size_t index) const { return tiles[index]; }
	bool empty() const noexcept { return tiles.empty(); }
	size_t size() const noexcept { return tiles.size(); }
};
