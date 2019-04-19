#pragma once

#include <memory>
#include "PhysFSStream.h"
#include <SFML/Graphics/Font.hpp>

class FreeTypeFont : public sf::Font
{
private:
	std::shared_ptr<sf::PhysFSStream> file;

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
};
