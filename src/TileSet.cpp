#include "TileSet.h"
#include "PhysFSStream.h"

TileSet::TileSet(const std::string& filename)
{
	sf::PhysFSStream file(filename);

	size_t numBlocks = (size_t)file.getSize() / (4 * 2);

	file.seek(0);

	TilBlock tmp(4);

	for (size_t i = 0; i < numBlocks; i++)
	{
		file.read(&tmp[0], 8);
		blocks.push_back(tmp);
	}
}
