#pragma once

#include <memory>
#include "PhysFSStream.h"
#include <SFML/Graphics/Font.hpp>

class FreeTypeFont : public sf::Font
{
private:
	std::shared_ptr<sf::PhysFSStream> file;
	sf::Color defaultColor{ sf::Color::White };

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
};
