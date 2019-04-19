#include "ColorLevelLayer.h"
#include <SFML/Graphics/RectangleShape.hpp>

void ColorLevelLayer::draw(sf::RenderTexture& levelTexture, const LevelLayerInfo& layerInfo) const
{
	if (background != sf::Color::Transparent)
	{
		sf::RectangleShape rectangle({ layerInfo.visibleRect.width, layerInfo.visibleRect.height });
		rectangle.setPosition({ layerInfo.visibleRect.left, layerInfo.visibleRect.top });
		rectangle.setFillColor(background);
		levelTexture.draw(rectangle);
	}
}
