#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <string>

class Shaders
{
private:
	static const std::string outlineText;
	static const std::string paletteText;

	static bool hasOutline;
	static bool hasPalette;

public:
	static sf::Shader Outline;
	static sf::Shader Palette;

	static bool supportsOutlines() { return hasOutline; }
	static bool supportsPalettes() { return hasPalette; }

	static void init();
};
