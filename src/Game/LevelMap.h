#pragma once

#include <cstdint>
#include "Dun.h"
#include <functional>
#include "LevelCell.h"
#include "LightMap.h"
#include "MapCoord.h"
#include "Sol.h"
#include "TileSet.h"
#include "Utils/Helper2D.h"
#include <vector>

class LevelMap
{
private:

	struct LightStruct
	{
		MapCoord mapPos;
		LightSource lightSource;
		bool remove{ false };
	};

	std::vector<LevelCell> cells;
	MapCoord mapSize;

	std::array<TileBlock, LevelCell::NumberOfLayers - 1> outOfBoundsTiles;

	// default tile size is the same for all map layers except automap
	int32_t defaultTileWidth{ 1 };
	int32_t defaultTileHeight{ 1 };
	// default block size - a tile is 4 blocks
	int32_t defaultBlockWidth{ 1 };
	int32_t defaultBlockHeight{ 1 };

	LightSource defaultSource;

	TileSet tileSet;
	Sol sol;
	LightMap lightMap;

	std::vector<LightStruct> pendingLights;

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

	// get the Tile's light for the layer, ignoring any object's light.
	// returns 0 if tile index is invalid.
	uint8_t getTileLight(size_t layer, const LevelCell& cell) const;

	// applies light function to map position.
	void doLight(MapCoord lightPos, LightSource lightSource,
		const std::function<void(LevelCell*, uint8_t)>& doLightFunc);

	// adds light to map position.
	void doDefaultLight(MapCoord lightPos, LightSource lightSource);

	// adds light to map position.
	void doLight(MapCoord lightPos, LightSource lightSource);

	// subtracts light from map position.
	void undoLight(MapCoord lightPos, LightSource lightSource);

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

	uint8_t getDefaultLight() const noexcept { return defaultSource.maxLight; }
	void setDefaultLightSource(LightSource light_) noexcept { defaultSource = light_; }

	void addLight(MapCoord lightPos, LightSource lightSource);
	void removeLight(MapCoord lightPos, LightSource lightSource);

	void loadLightMap(const std::string_view fileName) { lightMap.load(fileName, 0, 0xFFFF); }
	uint8_t getLight(size_t index) const { return std::max(getDefaultLight(), lightMap.get(index)); }

	void initLights();
	void updateLights();

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

	int32_t DefaultTileWidth() const noexcept { return defaultTileWidth; }
	int32_t DefaultTileHeight() const noexcept { return defaultTileHeight; }
	int32_t DefaultBlockWidth() const noexcept { return defaultBlockWidth; }
	int32_t DefaultBlockHeight() const noexcept { return defaultBlockHeight; }

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

	bool addLevelObject(std::unique_ptr<LevelObject> obj);
	bool removeLevelObject(const LevelObject* obj);

	template <class T>
	T* removeLevelObject(const MapCoord& mapCoord)
	{
		if (isMapCoordValid(mapCoord) == true)
		{
			auto obj = (*this)[mapCoord].removeObject<T>();
			if (obj != nullptr)
			{
				removeLight(mapCoord, obj->getLightSource());
				return obj;
			}
		}
		return nullptr;
	}

	std::vector<MapCoord> getPath(const MapCoord& a, const MapCoord& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
