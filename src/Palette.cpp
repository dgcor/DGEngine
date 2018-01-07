#include "Palette.h"
#include "PhysFSStream.h"
#include "Shaders.h"

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
	loadTexture();
}

Palette::Palette(const Palette& pal, const std::vector<sf::Uint8> trn)
{
	for (size_t i = 0; i < palette.size(); i++)
	{
		palette[i] = pal[trn[i]];
	}
	loadTexture();
}

void Palette::loadTexture()
{
	if (Shaders::supportsPalettes() == false)
	{
		return;
	}
	sf::Image img;
	img.create(palette.size(), 1, (const sf::Uint8*)&palette);
	texture.loadFromImage(img);
}
