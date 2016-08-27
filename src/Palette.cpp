#include "Palette.h"
#include "PhysFSStream.h"

Palette::Palette(const char* file)
{
	sf::PhysFSStream stream(file);

	if (stream.hasError() == true || stream.getSize() < 768)
	{
		return;
	}

	for (auto& color : palette)
	{
		stream.read(&color, 3);
	}
}

Palette::Palette(const Palette& pal, const std::vector<sf::Uint8> trn)
{
	for (size_t i = 0; i < palette.size(); i++)
	{
		palette[i] = pal[trn[i]];
	}
}
