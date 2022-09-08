#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/InputStream.hpp>

namespace ImageUtils
{
	bool LoadImagePCX(sf::InputStream& file, sf::Image& image);
}
