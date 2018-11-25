#include "Min.h"
#include "PhysFSStream.h"
#include "StreamReader.h"

Min::Min(const std::string_view fileName, size_t minSize)
{
	sf::PhysFSStream file(fileName.data());
	if (file.hasError() == true)
	{
		return;
	}

	auto fileSize = (size_t)file.getSize();
	numPillars = fileSize / (minSize * 2);
	pillarHeight = minSize / 2;

	pillars.resize(numPillars * pillarHeight);
	file.read(pillars.data(), file.getSize());

	LittleEndianStreamReader fileStream((const uint8_t*)pillars.data(), fileSize);

	MinPillar minPillar(pillars.data(), pillarHeight);
	size_t count = 0;
	for (auto& pillar : pillars)
	{
		std::pair<uint16_t, uint16_t> temp;
		fileStream.read(temp.first);
		fileStream.read(temp.second);
		pillar = temp;
		count++;
		if (count == pillarHeight)
		{
			if (minPillar.isTopBlank() == true)
			{
				numBlankTopPillars++;
			}
			count = 0;
			minPillar = MinPillar(&pillar + 1, pillarHeight);
		}
	}
}
