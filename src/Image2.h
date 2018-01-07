#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf
{
	class Image2 : public Image
	{
	public:
		operator Texture() const
		{
			auto tex = Texture();
			tex.loadFromImage(*this);
			return tex;
		}
	};
}
