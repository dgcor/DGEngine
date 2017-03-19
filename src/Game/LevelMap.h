#pragma once

#include <cstdint>
#include "Dun.h"
#include "Helper2D.h"
#include "LevelObject.h"
#include "MapCoord.h"
#include <queue>
#include "TileSet.h"
#include "Sol.h"
#include <vector>

struct LevelCell
{
	int16_t minIndex{ -1 };
	std::shared_ptr<LevelObject> object;
	int8_t sol{ 0 };

	bool PassableIgnoreObject() const
	{
		return !(sol & 0x01);
	}

	bool Passable() const
	{
		// Temporary logic
		bool passable = PassableIgnoreObject();
		if (object != nullptr)
		{
			return object->Passable();
		}
		return passable;
	}
};

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

	Coord Width() const { return mapSize.x; }
	Coord Height() const { return mapSize.y; }

	const MapCoord& MapSize() const { return mapSize; }

	static int TileSize() { return tileSize; }

	sf::Vector2f getCoord(const MapCoord& tile) const;
	MapCoord getTile(const sf::Vector2f& coords) const;

	std::queue<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;
};
