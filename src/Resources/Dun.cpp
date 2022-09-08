#include "Dun.h"
#include <algorithm>
#include "SFML/PhysFSStream.h"
#include "Utils/StreamReader.h"

Dun::Dun(const std::string_view fileName, int32_t defaultTile)
{
	sf::PhysFSStream file(fileName.data());
	if (file.hasError() == true)
	{
		return;
	}
	std::vector<uint8_t> fileData((size_t)file.getSize());
	file.read(fileData.data(), file.getSize());

	LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

	width = fileStream.read<uint16_t>();
	height = fileStream.read<uint16_t>();

	elements.resize(width * height);
	for (auto& elem : elements)
	{
		int16_t val;
		fileStream.read(val);
		elem = val;
	}
	std::transform(elements.begin(), elements.end(), elements.begin(), [](int32_t i) { return i - 1; });
	if (defaultTile >= 0)
	{
		std::replace(elements.begin(), elements.end(), -1, defaultTile);
	}
}
