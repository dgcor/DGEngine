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

TileSet::TileSet(const std::string& fileName)
{
	sf::PhysFSStream file(fileName);
	if (file.hasError() == true)
	{
		return;
	}
	std::vector<uint8_t> fileData((size_t)file.getSize());
	file.read(fileData.data(), file.getSize());

	LittleEndianStreamReader fileStream(fileData);

	size_t numTiles = fileData.size() / (4 * 2);

	tiles.resize(numTiles);
	for (auto& tile : tiles)
	{
		fileStream.read(std::get<0>(tile));
		fileStream.read(std::get<1>(tile));
		fileStream.read(std::get<2>(tile));
		fileStream.read(std::get<3>(tile));
	}
}
