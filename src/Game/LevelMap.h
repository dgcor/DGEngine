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

	std::array<TileBlock, LevelCell::NumberOfLayers - 1> outOfBoundsTiles;

	// default block size - a tile is 4 blocks
	int32_t defaultBlockWidth{ 1 };
	int32_t defaultBlockHeight{ 1 };

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
	LevelMap(const std::string_view tilFileName, const std::string_view solFileName,
		Coord width_, Coord height_, int16_t defaultTile = -1);
	LevelMap(Coord width_, Coord height_, int16_t defaultTile = -1);

	LevelMap& operator=(const LevelMap&) = default;
	LevelMap& operator=(LevelMap&&) = default;
	LevelMap(const LevelMap&) = default;
	LevelMap(LevelMap&&) = default;

	void resize(Coord width_, Coord height_, int16_t defaultTile = -1);

	void setDefaultTileSize(int32_t tileWidth_, int32_t tileHeight_) noexcept;

	void setTileSetArea(Coord x, Coord y, const Dun& dun);
	void setSimpleArea(Coord x, Coord y, const Dun& dun);
	void setSimpleArea(Coord x, Coord y, size_t layer,
		const Dun& dun, bool normalizeSolLayer = true);

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
	sf::Vector2f getCoord(const MapCoord& tile, int32_t blockWidth, int32_t blockHeight) const;
	MapCoord getTile(const sf::Vector2f& coords) const noexcept;
	MapCoord getTile(const sf::Vector2f& coords, int32_t blockWidth, int32_t blockHeight) const noexcept;

	int16_t getOutOfBoundsTileIndex(size_t layer, int32_t xCoord, int32_t yCoord) const noexcept
	{
		return outOfBoundsTiles[layer].getTileIndex(xCoord, yCoord);
	}

	void setOutOfBoundsTileIndex(size_t layer, int16_t tile) noexcept;

	std::vector<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
