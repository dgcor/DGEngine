#pragma once

#include <memory>
#include <SFML/Graphics/Font.hpp>
#include "SFML/PhysFSStream.h"

class FreeTypeFont : public sf::Font
{
private:
	std::shared_ptr<sf::PhysFSStream> file;
	sf::Color defaultColor{ sf::Color::White };
	unsigned int defaultCharSize{ 0 };

public:
	FreeTypeFont(const std::shared_ptr<sf::PhysFSStream>& file_) : file(file_) {}

	bool load()
	{
		if (file == nullptr || file->hasError() == true)
		{
			return false;
		}
		return loadFromStream(*file);
	}

	const sf::Color& getColor() const { return defaultColor; }
	void setColor(const sf::Color& color_) { defaultColor = color_; }

	const unsigned int getCharacterSize() const { return defaultCharSize; }
	void setCharacterSize(unsigned int size_) { defaultCharSize = size_; }
};
