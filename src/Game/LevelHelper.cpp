#include "LevelHelper.h"

namespace Render
{
	void drawFrame(sf::Image& s, int start_x, int start_y, const CelFrame& frame)
	{
		auto size = s.getSize();
		auto width = frame.Width();
		auto height = frame.Height();

		for (size_t x = 0; x < width; x++)
		{
			for (size_t y = 0; y < height; y++)
			{
				size_t xx = start_x + x;
				size_t yy = start_y + y;

				if (xx < size.x && yy < size.y)
				{
					s.setPixel(xx, yy, frame[x][y]);
				}
			}
		}
	}

	void drawMinTile(sf::Image& s, CelFrameCache& f, int x, int y, int16_t l, int16_t r)
	{
		if (l != -1)
			drawFrame(s, x, y, f[l]);

		if (r != -1)
			drawFrame(s, x + 32, y, f[r]);
	}

	void drawMinPillar(sf::Image& s, int x, int y, const std::vector<int16_t>& pillar, CelFrameCache& tileset, bool top)
	{
		// compensate for maps using 5-row min files
		if (pillar.size() == 10)
			y += 3 * 32;

		size_t i, lim;

		if (top)
		{
			i = 0;
			lim = pillar.size() - 2;
		}
		else
		{
			i = pillar.size() - 2;
			lim = pillar.size();
			y += i * 16;
		}

		// Each iteration draw one row of the min
		for (; i < lim; i += 2)
		{
			int16_t l = (pillar[i] & 0x0FFF) - 1;
			int16_t r = (pillar[i + 1] & 0x0FFF) - 1;

			drawMinTile(s, tileset, x, y, l, r);

			y += 32; // down 32 each row
		}
	}

	void drawMinPillarTop(sf::Image& s, int x, int y, const std::vector<int16_t>& pillar, CelFrameCache& tileset)
	{
		drawMinPillar(s, x, y, pillar, tileset, true);
	}

	void drawMinPillarBase(sf::Image& s, int x, int y, const std::vector<int16_t>& pillar, CelFrameCache& tileset)
	{
		drawMinPillar(s, x, y, pillar, tileset, false);
	}

	std::vector<std::shared_ptr<sf::Texture>> loadTilesetSprite(CelFrameCache& cel, Min& min, bool top)
	{
		std::vector<std::shared_ptr<sf::Texture>> newMin(min.size() - 1);

		for (size_t i = 0; i < min.size() - 1; i++)
		{
			sf::Image newPillar;
			newPillar.create(64, 256, sf::Color::Transparent);

			if (top)
			{
				drawMinPillarTop(newPillar, 0, 0, min[i], cel);
			}
			else
			{
				drawMinPillarBase(newPillar, 0, 0, min[i], cel);
			}

			auto tex = std::make_shared<sf::Texture>();
			tex->loadFromImage(newPillar);
			newMin[i] = tex;
		}

		return newMin;
	}

	void drawAt(const sf::Texture& tex, float x, float y, sf::RenderTarget& target, sf::RenderStates states)
	{
		sf::Sprite sprite(tex);
		sprite.setPosition(x, y);
		target.draw(sprite, states);
	}

	void getMapScreenCoords(unsigned width, unsigned height, const Level& level, int32_t x1, int32_t y1, int32_t x2, int32_t y2, size_t dist, int32_t& levelX, int32_t& levelY)
	{
		int32_t xPx1 = -((y1*(-32)) + 32 * x1 + (int)level.Width() * 32) + width / 2;
		int32_t yPx1 = -((y1 * 16) + (16 * x1) + 160) + height / 2;

		int32_t xPx2 = -((y2*(-32)) + 32 * x2 + (int)level.Width() * 32) + width / 2;
		int32_t yPx2 = -((y2 * 16) + (16 * x2) + 160) + height / 2;

		levelX = xPx1 + ((((float)(xPx2 - xPx1)) / 100.0)*(float)dist);
		levelY = yPx1 + ((((float)(yPx2 - yPx1)) / 100.0)*(float)dist);
	}

	sf::Vector2i getTileFromScreenCoords(const Level& level, size_t levelX, size_t levelY, size_t x, size_t y)
	{
		// Position on the map in pixels
		int32_t flatX = x - levelX;
		int32_t flatY = y - levelY;

		// position on the map divided into 32x16 flat blocks
		// every second one of these blocks is centred on an isometric
		// block centre, the others are centred on isometric block corners
		int32_t flatGridX = (flatX + 16) / 32;
		int32_t flatGridY = (flatY + 8) / 16;

		// origin position (in flat grid coords) for the first line (isometric y = 0)
		int32_t flatOriginPosX = level.Height();
		int32_t flatOriginPosY = 15;

		// when a flat grid box is clicked that does not centre on an isometric block, work out which
		// isometric quadrant of that box was clicked, then adjust flatGridPos accordingly
		if ((flatGridX % 2 == 1 && flatGridY % 2 == 1) || (flatGridX % 2 == 0 && flatGridY % 2 == 0))
		{
			// origin of current flat grid box
			int32_t baseX = 32 * flatGridX - 16;
			int32_t baseY = 16 * flatGridY - 8;

			// position within grid box
			int32_t blockPosX = flatX - baseX;
			int32_t blockPosY = flatY - baseY;

			if (blockPosY * 2 > blockPosX)
			{
				if (blockPosX < (15 - blockPosY) * 2)
				{
					flatGridX--;
				}
				else
				{
					flatGridY++;
				}
			}
			else
			{
				if (blockPosX < (15 - blockPosY) * 2)
				{
					flatGridY--;
				}
				else
				{
					flatGridX++;
				}
			}
		}

		// flatOrigin adjusted for the current y value
		int32_t lineOriginPosX = flatOriginPosX + ((flatGridX - flatOriginPosX) - (flatGridY - flatOriginPosY)) / 2;
		int32_t lineOriginPosY = flatOriginPosY - (-(flatGridX - flatOriginPosX) - (flatGridY - flatOriginPosY)) / 2;

		int32_t isoPosX = flatGridX - lineOriginPosX;
		int32_t isoPosY = flatGridY - lineOriginPosY;

		return sf::Vector2i(isoPosX, isoPosY);
	}

	sf::Vector2i getClickedTile(size_t levelWidth, size_t levelHeight, const Level& level, size_t x, size_t y, int32_t x1, int32_t y1, int32_t x2, int32_t y2, size_t dist)
	{
		int32_t levelX, levelY;
		getMapScreenCoords(levelWidth, levelHeight, level, x1, y1, x2, y2, dist, levelX, levelY);

		return getTileFromScreenCoords(level, levelX, levelY, x, y);
	}

	void drawLevelHelper(const Level& level, const std::vector<std::shared_ptr<sf::Texture>>& minSprites, int32_t x, int32_t y, int32_t levelX, int32_t levelY, sf::RenderTarget& target, sf::RenderStates states)
	{
		size_t index = level[x][y].minIndex;
		auto coords = level.Map().getCoords(sf::Vector2i(x, y));
		coords.x += levelX;
		coords.y += levelY;
		auto size = target.getView().getSize();

		if (coords.x >= -64 && coords.x <= size.x && coords.y >= -256 && coords.y <= size.y && index < minSprites.size())
		{
			drawAt(*minSprites[index], coords.x, coords.y, target, states);
		}
	}

	void drawLevelHelper2(const Level& level, const std::vector<std::shared_ptr<sf::Texture>>& minSprites,
		int32_t x, int32_t y, int32_t levelX, int32_t levelY,
		sf::RenderTarget& target, sf::RenderStates states)
	{
		size_t index = level[x][y].minIndex;
		auto coords = level.Map().getCoords(sf::Vector2i(x, y));
		coords.x += levelX;
		coords.y += levelY;
		auto size = target.getView().getSize();

		if (coords.x >= -64 && coords.x <= size.x && coords.y >= -256 && coords.y <= size.y)
		{
			auto drawObj = level.Map()[x][y].drawable;
			if (drawObj != nullptr)
			{
				target.draw(*drawObj, states);
			}
			if (index < minSprites.size())
			{
				drawAt(*minSprites[index], coords.x, coords.y, target, states);
			}
		}
	}
}
