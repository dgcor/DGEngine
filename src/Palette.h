#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Palette
{
private:
	std::array<sf::Color, 256> palette;

public:
	Palette() {}
	Palette(const std::string& file) : Palette(file.c_str()) {}
	Palette(const char* file);
	Palette(const Palette& pal, const std::vector<sf::Uint8> trn);

	const sf::Color& operator[](size_t index) const { return palette[index]; }
};
