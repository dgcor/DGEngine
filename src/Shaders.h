#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <string>

class Shaders
{
private:
	static const std::string outlineText;
	static const std::string paletteText;
	static const std::string gammaText;

	static bool hasOutline;
	static bool hasPalette;
	static bool hasGamma;

public:
	static sf::Shader Outline;
	static sf::Shader Palette;
	static sf::Shader Gamma;

	static bool supportsOutlines() noexcept { return hasOutline; }
	static bool supportsPalettes() noexcept { return hasPalette; }
	static bool supportsGamma() noexcept { return hasGamma; }

	static void init();
};
