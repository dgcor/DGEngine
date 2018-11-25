#include "TileSet.h"
#include "PhysFSStream.h"
#include "StreamReader.h"

int16_t TileBlock::getTileIndex(int32_t xCoord, int32_t yCoord) const noexcept
{
	if (xCoord % 2)
	{
		if (yCoord % 2)
			return std::get<3>(*this);
		else
			return std::get<1>(*this);
	}
	else
	{
		if (yCoord % 2)
			return std::get<2>(*this);
		else
			return std::get<0>(*this);
	}
}

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
		fileStream.read(std::get<0>(temp));
		fileStream.read(std::get<1>(temp));
		fileStream.read(std::get<2>(temp));
		fileStream.read(std::get<3>(temp));
		tile = temp;
	}
}
