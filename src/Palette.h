#pragma once

#include <array>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>

typedef std::array<sf::Color, 256> PaletteArray;

class Palette
{
private:
	void loadTexture();

public:
	sf::Texture texture;
	PaletteArray palette;

	Palette() {}
	Palette(const std::string& file) : Palette(file.c_str()) {}
	Palette(const char* file);
	Palette(const Palette& pal, const std::vector<sf::Uint8> trn);

	const sf::Color& operator[](size_t index) const { return palette[index]; }
};
