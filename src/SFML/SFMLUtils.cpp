#include "SFMLUtils.h"
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics/Texture.hpp>

namespace SFMLUtils
{
	sf::Color rgbToColor(unsigned val)
	{
		sf::Uint8 r = (val & 0x00FF0000) >> 16;
		sf::Uint8 g = (val & 0x0000FF00) >> 8;
		sf::Uint8 b = (val & 0x000000FF);
		return sf::Color(r, g, b);
	}

	sf::Color rgbaToColor(unsigned val)
	{
		sf::Uint8 r = (val & 0xFF000000) >> 24;
		sf::Uint8 g = (val & 0x00FF0000) >> 16;
		sf::Uint8 b = (val & 0x0000FF00) >> 8;
		sf::Uint8 a = (val & 0x000000FF);
		return sf::Color(r, g, b, a);
	}

	sf::Color stringToColor(const std::string_view str)
	{
		auto num = (unsigned)std::strtoul(str.data(), nullptr, 16);
		if (str.length() >= 10)
		{
			return rgbaToColor(num);
		}
		else
		{
			return rgbToColor(num);
		}
	}

	void spriteCenterTexture(sf::Sprite& sprite)
	{
		sf::Vector2f origin;
		auto texSize = sprite.getTexture()->getSize();
		auto texRect = sprite.getTextureRect();
		bool updateTexRect = false;

		float valX = std::round(-((float)texRect.width / 2.f) + ((float)texSize.x / 2.f));
		if ((int)texSize.x <= texRect.width)
		{
			origin.x = valX;
		}
		else
		{
			texRect.left = (int)valX;
			updateTexRect = true;
		}
		float valY = std::round(-((float)texRect.height / 2.f) + ((float)texSize.y / 2.f));
		if ((int)texSize.y <= texRect.height)
		{
			origin.y = valY;
		}
		else
		{
			texRect.top = (int)valY;
			updateTexRect = true;
		}
		sprite.setOrigin(origin);
		if (updateTexRect == true)
		{
			sprite.setTextureRect(texRect);
		}
	}

	sf::BlendMode getBlendMode(BlendMode blendMode)
	{
		switch (blendMode)
		{
		default:
		case BlendMode::Alpha:
			return sf::BlendAlpha;
		case BlendMode::Add:
			return sf::BlendAdd;
		case BlendMode::Multiply:
			return sf::BlendMultiply;
		case BlendMode::None:
			return sf::BlendNone;
		}
	}

	float getScaleToStretchAndKeepAR(const sf::Vector2u& size,
		const sf::Vector2u& windowSize)
	{
		float windowRatio = (float)windowSize.x / (float)windowSize.y;
		float drawableRatio = (float)size.x / (float)size.y;
		bool horizontalSpacing = true;
		if (windowRatio < drawableRatio)
		{
			horizontalSpacing = false;
		}
		if (horizontalSpacing == true)
		{
			return (float)windowSize.x / (float)size.x;
		}
		else
		{
			return (float)windowSize.y / (float)size.y;
		}
	}

	void viewStretchKeepAR(sf::View& view, const sf::Vector2u& windowSize, sf::FloatRect viewPort)
	{
		// Compares the aspect ratio of the window to the aspect ratio of the view,
		// and sets the view's viewport accordingly in order to archieve a letterbox effect.
		// A new view (with a new viewport set) is returned.

		float windowRatio = (float)windowSize.x / (float)windowSize.y;
		float viewRatio = view.getSize().x / view.getSize().y;

		bool horizontalSpacing = true;
		if (windowRatio < viewRatio)
		{
			horizontalSpacing = false;
		}

		// If horizontalSpacing is true, the black bars will appear on the left and right side.
		// Otherwise, the black bars will appear on the top and bottom.

		if (horizontalSpacing == true)
		{
			viewPort.width = viewRatio / windowRatio;
			viewPort.left = (1.f - viewPort.width) / 2.f;
		}
		else
		{
			viewPort.height = windowRatio / viewRatio;
			viewPort.top = (1.f - viewPort.height) / 2.f;
		}
		view.setViewport(viewPort);
	}
}
