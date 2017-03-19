#include "SFMLUtils.h"
#include <cmath>

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

	sf::Color stringToColor(const std::string& str)
	{
		if (str.length() >= 10)
		{
			return rgbaToColor(std::stoul(str, 0, 16));
		}
		else
		{
			return rgbToColor(std::stoul(str, 0, 16));
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

	void viewStretchKeepAR(sf::View& view, const sf::Vector2u& windowSize, sf::FloatRect viewPort)
	{
		// Compares the aspect ratio of the window to the aspect ratio of the view,
		// and sets the view's viewport accordingly in order to archieve a letterbox effect.
		// A new view (with a new viewport set) is returned.

		float windowRatio = (float)windowSize.x / (float)windowSize.y;
		float viewRatio = view.getSize().x / (float)view.getSize().y;

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
