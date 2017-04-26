#pragma once

#include <cstdint>
#include "Dun.h"
#include "Helper2D.h"
#include "LevelCell.h"
#include "MapCoord.h"
#include "TileSet.h"
#include "Sol.h"
#include <vector>

class LevelMap
{
private:
	static int tileSize;

	std::vector<LevelCell> cells;
	MapCoord mapSize;

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
	LevelMap() {}
	LevelMap(Coord width_, Coord height_);

	void setArea(Coord x, Coord y, const Dun& dun, const TileSet& til, const Sol& sol);

	Misc::Helper2D<LevelMap, LevelCell&, Coord> operator[] (Coord x)
	{
		return Misc::Helper2D<LevelMap, LevelCell&, Coord>(*this, x, get);
	}
	Misc::Helper2D<const LevelMap, const LevelCell&, Coord> operator[] (Coord x) const
	{
		return Misc::Helper2D<const LevelMap, const LevelCell&, Coord>(*this, x, get);
	}

	LevelCell& operator[] (MapCoord coord) { return get(coord.x, coord.y, *this); }
	const LevelCell& operator[] (MapCoord coord) const { return get(coord.x, coord.y, *this); }

	Coord Width() const { return mapSize.x; }
	Coord Height() const { return mapSize.y; }

	const MapCoord& MapSize() const { return mapSize; }

	static int TileSize() { return tileSize; }

	sf::Vector2f getCoord(const MapCoord& tile) const;
	MapCoord getTile(const sf::Vector2f& coords) const;

	std::vector<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;
};
