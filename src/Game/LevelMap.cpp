#include "LevelMap.h"
#include "PathFinder.h"

int LevelMap::tileSize = 32;

LevelMap::LevelMap(Coord width_, Coord height_) : mapSize(width_, height_)
{
	if (mapSize.x == std::numeric_limits<Coord>::max())
	{
		mapSize.x--;
	}
	if (mapSize.y == std::numeric_limits<Coord>::max())
	{
		mapSize.y--;
	}
	cells.resize(mapSize.x * mapSize.y);
}

void LevelMap::setArea(Coord x, Coord y, const Dun& dun, const TileSet& til, const Sol& sol)
{
	auto dWidth = dun.Width() * 2;
	auto dHeight = dun.Height() * 2;
	for (size_t i = 0; i < dWidth; i++)
	{
		for (size_t j = 0; j < dHeight; j++)
		{
			size_t xDunIndex = i;
			size_t xTilIndex = 0;
			if ((xDunIndex % 2) != 0)
			{
				xDunIndex--;
				xTilIndex = 1;
			}
			xDunIndex /= 2;

			size_t yDunIndex = j;
			size_t yTilIndex = 0;
			if ((yDunIndex % 2) != 0)
			{
				yDunIndex--;
				yTilIndex = 1;
			}
			yDunIndex /= 2;

			size_t tilIndex;

			if (xTilIndex)
			{
				if (yTilIndex)
					tilIndex = 3; // bottom
				else
					tilIndex = 1; // left
			}
			else
			{
				if (yTilIndex)
					tilIndex = 2; // right
				else
					tilIndex = 0; // top
			}

			int32_t dunIndex = dun[xDunIndex][yDunIndex] - 1;

			auto cellX = x + i;
			auto cellY = y + j;

			if (cellX >= mapSize.x || cellY >= mapSize.y)
			{
				continue;
			}

			auto& cell = cells[cellX + (cellY * mapSize.x)];

			if (dunIndex == -1)
			{
				cell.MinIndex(-1);
				cell.Sol(0);
			}
			else
			{
				cell.MinIndex(til[dunIndex][tilIndex]);
				cell.Sol(sol.get(cell.MinIndex()));
			}
		}
	}
}

sf::Vector2f LevelMap::getCoord(const MapCoord& tile) const
{
	return sf::Vector2f(
		(float)((tile.y*(-32)) + 32 * tile.x + mapSize.y * 32 - 32),
		(float)((tile.y * 16) + 16 * tile.x));
}

MapCoord LevelMap::getTile(const sf::Vector2f& coords) const
{
	// Position on the map in pixels
	int32_t flatX = (int32_t)coords.x;
	int32_t flatY = (int32_t)coords.y;

	// position on the map divided into 32x16 flat blocks
	// every second one of these blocks is centred on an isometric
	// block centre, the others are centred on isometric block corners
	int32_t flatGridX = (flatX + 16) / 32;
	int32_t flatGridY = (flatY + 8) / 16;

	// origin position (in flat grid coords) for the first line (isometric y = 0)
	int32_t flatOriginPosX = mapSize.y;
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

	return MapCoord((Coord)isoPosX, (Coord)isoPosY);
}

std::queue<MapCoord> LevelMap::getPath(const MapCoord& a, const MapCoord& b) const
{
	std::queue<MapCoord> path;

	MapSearchNode start(this, a.x, a.y, PlayerDirection::All);
	MapSearchNode end(this, b.x, b.y, PlayerDirection::All);

	if (end.IsPassableIgnoreObject() == false)
	{
		return path;
	}

	PathFinder pathFinder(PATH_FINDER_MAX);
	pathFinder.SetStartAndGoalStates(start, end);

	unsigned int SearchState;

	do
	{
		SearchState = pathFinder.SearchStep();

		auto StepCount = pathFinder.GetStepCount();
		if (StepCount == PATH_FINDER_MAX)
		{
			pathFinder.CancelSearch();
		}
	} while (SearchState == PathFinder::SEARCH_STATE_SEARCHING);

	if (SearchState == PathFinder::SEARCH_STATE_SUCCEEDED)
	{
		auto node = pathFinder.GetSolutionStart();

		while (true)
		{
			if (node == nullptr)
			{
				break;
			}
			path.push(MapCoord(node->x, node->y));
			node = pathFinder.GetSolutionNext();
		};
		pathFinder.FreeSolutionNodes();
	}
	pathFinder.EnsureMemoryFreed();

	return path;
}
