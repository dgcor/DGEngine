#include <iostream>
#include <complex>

#include <SFML/Graphics.hpp>
#include "Min.h"
#include "Level.h"
#include "CelCache.h"

namespace Render
{
	void getMapScreenCoords(unsigned width, unsigned height,
		const Level& level, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		size_t dist, int32_t& levelX, int32_t& levelY);

	void drawLevelHelper(const Level& level,
		const std::vector<std::shared_ptr<sf::Texture>>& minSprites,
		int32_t x, int32_t y, int32_t levelX, int32_t levelY,
		sf::RenderTarget& target, sf::RenderStates states);

	void drawLevelHelper2(const Level& level,
		const std::vector<std::shared_ptr<sf::Texture>>& minSprites,
		int32_t x, int32_t y, int32_t levelX, int32_t levelY,
		sf::RenderTarget& target, sf::RenderStates states);

	std::vector<std::shared_ptr<sf::Texture>> loadTilesetSprite(CelFrameCache& cel, Min& min, bool top);

	sf::Vector2i getClickedTile(size_t levelWidth, size_t levelHeight,
		const Level& level, size_t x, size_t y, int32_t x1, int32_t y1,
		int32_t x2, int32_t y2, size_t dist);
}
