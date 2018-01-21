#pragma once

#include <cstdint>
#include "Dun.h"
#include "LevelCell.h"
#include "MapCoord.h"
#include "TileSet.h"
#include "Sol.h"
#include "Utils/Helper2D.h"
#include <vector>

class LevelMap
{
private:
	std::vector<LevelCell> cells;
	MapCoord mapSize;

	// a tile is 4 blocks
	int32_t blockWidth{ 0 };
	int32_t blockHeight{ 0 };

	using Coord = decltype(mapSize.x);

	static const LevelCell& get(Coord x, Coord y, const LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}
	static LevelCell& get(Coord x, Coord y, LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}

public:
	LevelMap() noexcept {}
	LevelMap(Coord width_, Coord height_);

	void setTileSize(int32_t tileWidth_, int32_t tileHeight_) noexcept;

	void setArea(Coord x, Coord y, const Dun& dun, const TileSet& til, const Sol& sol);

	void setArea(Coord x, Coord y, const Dun& dun, const Sol& sol);

	Misc::Helper2D<LevelMap, LevelCell&, Coord> operator[] (Coord x) noexcept
	{
		return Misc::Helper2D<LevelMap, LevelCell&, Coord>(*this, x, get);
	}
	Misc::Helper2D<const LevelMap, const LevelCell&, Coord> operator[] (Coord x) const noexcept
	{
		return Misc::Helper2D<const LevelMap, const LevelCell&, Coord>(*this, x, get);
	}

	LevelCell& operator[] (MapCoord coord) { return get(coord.x, coord.y, *this); }
	const LevelCell& operator[] (MapCoord coord) const { return get(coord.x, coord.y, *this); }

	Coord Width() const noexcept { return mapSize.x; }
	Coord Height() const noexcept { return mapSize.y; }

	const MapCoord& MapSize() const noexcept { return mapSize; }

	bool isMapCoordValid(const MapCoord& mapCoord) const noexcept;

	sf::Vector2f getCoord(const MapCoord& tile) const;
	MapCoord getTile(const sf::Vector2f& coords) const noexcept;

	std::vector<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
