#include "LevelHelper.h"

namespace LevelHelper
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

	void drawMinPillar(sf::Image& s, int x, int y,
		const std::vector<int16_t>& pillar, CelFrameCache& tileset, bool top)
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

	void drawMinPillarTop(sf::Image& s, int x, int y,
		const std::vector<int16_t>& pillar, CelFrameCache& tileset)
	{
		drawMinPillar(s, x, y, pillar, tileset, true);
	}

	void drawMinPillarBase(sf::Image& s, int x, int y,
		const std::vector<int16_t>& pillar, CelFrameCache& tileset)
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
}
