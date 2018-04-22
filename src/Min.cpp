#include "Min.h"
#include "PhysFSStream.h"
#include "StreamReader.h"

Min::Min(const std::string& fileName, size_t minSize)
{
	sf::PhysFSStream file(fileName);
	if (file.hasError() == true)
	{
		return;
	}
	std::vector<uint8_t> fileData((size_t)file.getSize());
	file.read(fileData.data(), file.getSize());

	LittleEndianStreamReader fileStream(fileData);

	auto numPillars = fileData.size() / (minSize * 2);
	auto pillarHeight = minSize / 2;

	pillars.resize(numPillars);
	for (auto& pillar : pillars)
	{
		pillar.resize(pillarHeight);
		for (auto& tile : pillar)
		{
			fileStream.read(tile.first);
			fileStream.read(tile.second);
		}
	}
}
