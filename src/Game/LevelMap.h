#pragma once

#include <cstdint>
#include "Dun.h"
#include <functional>
#include "LevelCell.h"
#include "LightMap.h"
#include "PairXY.h"
#include "Sol.h"
#include "TileSet.h"
#include "Utils/Helper2D.h"
#include <vector>

class LevelMap
{
private:

	struct LightStruct
	{
		PairInt32 mapPos;
		LightSource lightSource;
		bool remove{ false };
	};

	std::vector<LevelCell> cells;
	PairInt32 mapSizei;
	PairFloat mapSizef;

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

	static const LevelCell& get(int32_t x, int32_t y, const LevelMap& map)
	{
		return map.cells[x + y * map.mapSizei.x];
	}
	static LevelCell& get(int32_t x, int32_t y, LevelMap& map)
	{
		return map.cells[x + y * map.mapSizei.x];
	}

	static const LevelCell& get(float x, float y, const LevelMap& map)
	{
		return get((int32_t)x, (int32_t)y, map);
	}
	static LevelCell& get(float x, float y, LevelMap& map)
	{
		return get((int32_t)x, (int32_t)y, map);
	}

	// get the Tile's light for the layer, ignoring any object's light.
	// returns 0 if tile index is invalid.
	uint8_t getTileLight(size_t layer, const LevelCell& cell) const;

	// applies light function to map position.
	void doLight(PairInt32 lightPos, LightSource lightSource,
		const std::function<void(LevelCell*, uint8_t)>& doLightFunc);

	// adds light to map position.
	void doDefaultLight(PairInt32 lightPos, LightSource lightSource);

	// adds light to map position.
	void doLight(PairInt32 lightPos, LightSource lightSource);

	// subtracts light from map position.
	void undoLight(PairInt32 lightPos, LightSource lightSource);

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
		int32_t width_, int32_t height_, int16_t defaultTile = -1);
	LevelMap(int32_t width_, int32_t height_, int16_t defaultTile = -1);

	LevelMap& operator=(const LevelMap&) = default;
	LevelMap& operator=(LevelMap&&) = default;
	LevelMap(const LevelMap&) = default;
	LevelMap(LevelMap&&) = default;

	void resize(int32_t width_, int32_t height_);

	// clears map and keeps size
	void clear(int16_t defaultTile = -1);

	void setDefaultTileSize(int32_t tileWidth_, int32_t tileHeight_) noexcept;

	uint8_t getDefaultLight() const noexcept { return defaultSource.maxLight; }
	void setDefaultLightSource(LightSource light_) noexcept { defaultSource = light_; }

	void addLight(PairInt32 lightPos, LightSource lightSource);
	void addLight(PairFloat lightPos, LightSource lightSource);
	void removeLight(PairInt32 lightPos, LightSource lightSource);
	void removeLight(PairFloat lightPos, LightSource lightSource);

	void loadLightMap(const std::string_view fileName) { lightMap.load(fileName, 0, 0xFFFF); }
	uint8_t getLight(size_t index) const { return std::max(getDefaultLight(), lightMap.get(index)); }

	void initLights();
	void updateLights();

	// sets area (tileBlock Dun file) for layer 0 and uses the Sol file to set the Sol layer.
	void setTileSetAreaUseSol(int32_t x, int32_t y, const Dun& dun);

	// sets area (indexes Dun file) for layer n and uses the Sol file to set the Sol layer.
	void setSimpleAreaUseSol(size_t layer, int32_t x, int32_t y, const Dun& dun);

	// sets area (indexes Dun file)
	void setSimpleArea(size_t layer, int32_t x, int32_t y,
		const Dun& dun, bool normalizeSolLayer = true);

	Misc::Helper2D<LevelMap, LevelCell&, int32_t> operator[] (int32_t x) noexcept
	{
		return Misc::Helper2D<LevelMap, LevelCell&, int32_t>(*this, x, get);
	}
	Misc::Helper2D<const LevelMap, const LevelCell&, int32_t> operator[] (int32_t x) const noexcept
	{
		return Misc::Helper2D<const LevelMap, const LevelCell&, int32_t>(*this, x, get);
	}

	LevelCell& operator[] (const PairInt32& coord) { return get(coord.x, coord.y, *this); }
	const LevelCell& operator[] (const PairInt32& coord) const { return get(coord.x, coord.y, *this); }

	LevelCell& operator[] (const PairFloat& coord) { return get(coord.x, coord.y, *this); }
	const LevelCell& operator[] (const PairFloat& coord) const { return get(coord.x, coord.y, *this); }

	const PairInt32& MapSizei() const noexcept { return mapSizei; }
	const PairFloat& MapSizef() const noexcept { return mapSizef; }

	int32_t DefaultTileWidth() const noexcept { return defaultTileWidth; }
	int32_t DefaultTileHeight() const noexcept { return defaultTileHeight; }
	int32_t DefaultBlockWidth() const noexcept { return defaultBlockWidth; }
	int32_t DefaultBlockHeight() const noexcept { return defaultBlockHeight; }

	bool isLayerUsed(size_t layer) const noexcept;

	bool isMapCoordValid(int32_t x, int32_t y) const noexcept;
	bool isMapCoordValid(const PairInt32& mapCoord) const noexcept;

	bool isMapCoordValid(float x, float y) const noexcept;
	bool isMapCoordValid(const PairFloat& mapCoord) const noexcept;

	template <class T>
	sf::Vector2f toDrawCoord(const T& mapPos) const
	{
		return toDrawCoord(mapPos, defaultBlockWidth, defaultBlockHeight);
	}

	template <class T>
	sf::Vector2f toDrawCoord(const T& mapPos, int32_t blockWidth, int32_t blockHeight) const
	{
		return sf::Vector2f(
			(float)std::round((mapPos.x - mapPos.y) * blockWidth),
			(float)std::round((mapPos.x + mapPos.y) * blockHeight)
		);
	}

	PairFloat toMapCoord(const sf::Vector2f& drawCoord) const noexcept;
	PairFloat toMapCoord(const sf::Vector2f& drawCoord, int32_t blockWidth, int32_t blockHeight) const noexcept;

	TileBlock getTileBlock(int16_t index) const;

	bool addLevelObject(std::unique_ptr<LevelObject> obj);
	bool removeLevelObject(const LevelObject* obj);

	template <class T, class MapCoord_>
	T* removeLevelObject(const MapCoord_& mapCoord)
	{
		if (isMapCoordValid(mapCoord) == true)
		{
			auto obj = (*this)[mapCoord].template removeObject<T>();
			if (obj != nullptr)
			{
				removeLight(mapCoord, obj->getLightSource());
				return obj;
			}
		}
		return nullptr;
	}

	std::vector<PairFloat> getPath(const PairFloat& a, const PairFloat& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
