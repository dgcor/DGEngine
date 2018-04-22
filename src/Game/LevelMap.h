#pragma once

#include <cstdint>
#include "Dun.h"
#include "LevelCell.h"
#include "MapCoord.h"
#include "Sol.h"
#include "TileSet.h"
#include "Utils/Helper2D.h"
#include <vector>

class LevelMap
{
private:
	std::vector<LevelCell> cells;
	MapCoord mapSize;

	TileBlock outOfBoundsTileBack;
	TileBlock outOfBoundsTileFront;

	// a tile is 4 blocks
	int32_t blockWidth{ 0 };
	int32_t blockHeight{ 0 };

	TileSet tileSet;
	Sol sol;

	using Coord = decltype(mapSize.x);

	static const LevelCell& get(Coord x, Coord y, const LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}
	static LevelCell& get(Coord x, Coord y, LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}

	void resize(int16_t defaultTile);
	void resize(const TileBlock& defaultTile);

public:
	using iterator = std::vector<LevelCell>::iterator;
	using const_iterator = std::vector<LevelCell>::const_iterator;
	using reverse_iterator = std::vector<LevelCell>::reverse_iterator;
	using const_reverse_iterator = std::vector<LevelCell>::const_reverse_iterator;

	iterator begin() noexcept { return cells.begin(); }
	iterator end() noexcept { return cells.end(); }
	const_iterator begin() const noexcept { return cells.begin(); }
	const_iterator end() const noexcept { return cells.end(); }
	const_iterator cbegin() const noexcept { return cells.cbegin(); }
	const_iterator cend() const noexcept { return cells.cend(); }
	reverse_iterator rbegin() noexcept { return cells.rbegin(); }
	reverse_iterator rend() noexcept { return cells.rend(); }
	const_reverse_iterator rbegin() const noexcept { return cells.rbegin(); }
	const_reverse_iterator rend() const noexcept { return cells.rend(); }
	const_reverse_iterator crbegin() const noexcept { return cells.crbegin(); }
	const_reverse_iterator crend() const noexcept { return cells.crend(); }

	LevelMap() noexcept {}
	LevelMap(const std::string& tilFileName, const std::string& solFileName,
		Coord width_, Coord height_, int16_t defaultTile = -1);
	LevelMap(Coord width_, Coord height_, int16_t defaultTile = -1);

	void resize(Coord width_, Coord height_, int16_t defaultTile = -1);

	void setTileSize(int32_t tileWidth_, int32_t tileHeight_) noexcept;

	void setTileSetArea(Coord x, Coord y, const Dun& dun);
	void setSimpleArea(Coord x, Coord y, const Dun& dun);
	void setSimpleArea(Coord x, Coord y, size_t index, const Dun& dun);

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

	bool isMapCoordValid(Coord x, Coord y) const noexcept;
	bool isMapCoordValid(const MapCoord& mapCoord) const noexcept;

	sf::Vector2f getCoord(const MapCoord& tile) const;
	MapCoord getTile(const sf::Vector2f& coords) const noexcept;

	int16_t getOutOfBoundsTileIndexBack(int32_t xCoord, int32_t yCoord) const noexcept
	{
		return outOfBoundsTileBack.getTileIndex(xCoord, yCoord);
	}
	int16_t getOutOfBoundsTileIndexFront(int32_t xCoord, int32_t yCoord) const noexcept
	{
		return outOfBoundsTileFront.getTileIndex(xCoord, yCoord);
	}

	void setOutOfBoundsTileBack(int16_t tile) noexcept;
	void setOutOfBoundsTileFront(int16_t tile) noexcept;

	std::vector<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
