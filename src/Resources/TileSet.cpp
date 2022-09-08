#include "TileSet.h"
#include "SFML/PhysFSStream.h"
#include "Utils/StreamReader.h"

TileSet::TileSet(const std::string_view fileName)
{
	sf::PhysFSStream file(fileName.data());
	if (file.hasError() == true)
	{
		return;
	}

	auto fileSize = (size_t)file.getSize();
	size_t numTiles = fileSize / (4 * 2);

	tiles.resize(numTiles);
	file.read(tiles.data(), file.getSize());

	LittleEndianStreamReader fileStream((const uint8_t*)tiles.data(), fileSize);

	for (auto& tile : tiles)
	{
		TileBlock temp;
		fileStream.read(temp.get<0>());
		fileStream.read(temp.get<1>());
		fileStream.read(temp.get<2>());
		fileStream.read(temp.get<3>());
		tile = temp;
	}
}
