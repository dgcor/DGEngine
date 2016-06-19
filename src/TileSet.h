#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TileSet
{
private:
	typedef std::vector<int16_t> TilBlock;
	std::vector<TilBlock> blocks;

public:
	TileSet(const std::string& fileName);
	const TilBlock& operator[] (size_t index) const { return blocks[index]; }
	size_t size() const { return blocks.size(); }
};
