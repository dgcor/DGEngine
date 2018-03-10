#pragma once

#include <memory>
#include <SFML/Graphics/Font.hpp>
#include "PhysFSStream.h"

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
