#include "LevelMap.h"
#include "PathFinder.h"

int LevelMap::tileSize = 32;

LevelMap::LevelMap(size_t width_, size_t height_) : width(width_), height(height_)
{
	cells.resize(width_ * height_);
	size.x = width_ * tileSize;
	size.y = height_ * tileSize;
}

void LevelMap::setArea(size_t x, size_t y, const Dun& dun, const TileSet& til, const Sol& sol)
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

			if (cellX >= width || cellY >= height)
			{
				continue;
			}

			auto& cell = cells[cellX + (cellY * width)];

			if (dunIndex == -1)
			{
				cell.minIndex = -1;
				cell.sol = 0;
			}
			else
			{
				cell.minIndex = til[dunIndex][tilIndex];
				cell.sol = sol.get(cell.minIndex);
			}
		}
	}
}

sf::Vector2f LevelMap::getCoords(const sf::Vector2i& tile) const
{
	return sf::Vector2f(
		(tile.y*(-32)) + 32 * tile.x + height * 32 - 32,
		(tile.y * 16) + 16 * tile.x);
}

sf::Vector2i LevelMap::getTile(const sf::Vector2f& coords) const
{
	return sf::Vector2i();
}

std::queue<sf::Vector2i> LevelMap::getPath(const sf::Vector2i& a, const sf::Vector2i& b)
{
	std::queue<sf::Vector2i> path;

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
	}
	while (SearchState == PathFinder::SEARCH_STATE_SEARCHING);

	if (SearchState == PathFinder::SEARCH_STATE_SUCCEEDED)
	{
		auto node = pathFinder.GetSolutionStart();

		while (true)
		{
			if (node == nullptr)
			{
				break;
			}
			path.emplace(sf::Vector2i(node->x, node->y));
			node = pathFinder.GetSolutionNext();
		};
		pathFinder.FreeSolutionNodes();
	}
	pathFinder.EnsureMemoryFreed();

	return path;
}
