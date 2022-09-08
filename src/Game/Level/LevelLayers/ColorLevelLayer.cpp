#include "ColorLevelLayer.h"
#include "Game/Level/LevelSurface.h"
#include <SFML/Graphics/RectangleShape.hpp>

void ColorLevelLayer::draw(const LevelSurface& surface) const
{
	if (background != sf::Color::Transparent)
	{
		sf::RectangleShape rectangle({ surface.visibleRect.width, surface.visibleRect.height });
		rectangle.setPosition({ surface.visibleRect.left, surface.visibleRect.top });
		rectangle.setFillColor(background);
		surface.draw(rectangle);
	}
}
