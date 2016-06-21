#include "Min.h"
#include "PhysFSStream.h"

Min::Min(const std::string& filename, size_t minSize)
{
	sf::PhysFSStream file(filename);

	if (file.hasError() == true)
	{
		return;
	}

	auto numPillars = file.getSize() / (minSize * 2);

	file.seek(0);

	std::vector<int16_t> temp(minSize);

	for (size_t i = 0; i < numPillars; i++)
	{
		if (file.read(&temp[0], minSize * 2) > 0)
		{
			pillars.push_back(std::vector<int16_t>(temp));
		}
	}
}
