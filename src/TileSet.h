#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

class TileBlock : public std::tuple<int16_t, int16_t, int16_t, int16_t>
{
public:
	TileBlock() : std::tuple<int16_t, int16_t, int16_t, int16_t>(-1, -1, -1, -1) {}
	TileBlock(int16_t tile) : std::tuple<int16_t, int16_t, int16_t, int16_t>(tile, tile, tile, tile) {}

	int16_t getTileIndex(int32_t xCoord, int32_t yCoord) const noexcept;
};

class TileSet
{
private:
	std::vector<TileBlock> tiles;

public:
	TileSet() {}
	TileSet(const std::string& fileName);
	const TileBlock& operator[] (size_t index) const { return tiles[index]; }
	bool empty() const noexcept { return tiles.empty(); }
	size_t size() const noexcept { return tiles.size(); }
};
