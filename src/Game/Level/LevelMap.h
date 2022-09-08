#pragma once

#include <cstdint>
#include "FlagsVector.h"
#include "Game/LightMap.h"
#include "LevelCell.h"
#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
#include "Resources/DS1.h"
#endif
#include "Resources/Dun.h"
#include "Resources/TileSet.h"
#include "Utils/Helper2D.h"
#include "Utils/PairXY.h"
#include <vector>

class LevelMap
{
private:
	static constexpr uint32_t MaxNumberOfLightsToUse = 512u;

	struct LightStruct
	{
		PairFloat mapPos;
		sf::Vector2f drawPos;
		LightSource lightSource;
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

	uint32_t defaultSubTiles{ 1 };

	LightSource defaultLight;

	TileSet tileSet;
	std::variant<FlagsVector, std::weak_ptr<LevelFlags>> flagsVariant;
	LightMap lightMap;

	std::vector<LightStruct> mapLights;
	std::vector<LightStruct> allLights;
	static uint32_t maxLights;
	bool lightsNeedUpdate{ false };

	static auto& get(int32_t x, int32_t y, const LevelMap& map) { return map.cells[x + y * map.mapSizei.x]; }
	static auto& get(int32_t x, int32_t y, LevelMap& map) { return map.cells[x + y * map.mapSizei.x]; }

	static auto& get(float x, float y, const LevelMap& map) { return get((int32_t)x, (int32_t)y, map); }
	static auto& get(float x, float y, LevelMap& map) { return get((int32_t)x, (int32_t)y, map); }

	const LevelFlags* getFlags();
	void getSubIndex(int32_t x, int32_t y, uint32_t& subIndex);

	void updateMapLights();

public:
	auto begin() noexcept { return cells.begin(); }
	auto end() noexcept { return cells.end(); }
	auto begin() const noexcept { return cells.begin(); }
	auto end() const noexcept { return cells.end(); }
	auto cbegin() const noexcept { return cells.cbegin(); }
	auto cend() const noexcept { return cells.cend(); }
	auto rbegin() noexcept { return cells.rbegin(); }
	auto rend() noexcept { return cells.rend(); }
	auto rbegin() const noexcept { return cells.rbegin(); }
	auto rend() const noexcept { return cells.rend(); }
	auto crbegin() const noexcept { return cells.crbegin(); }
	auto crend() const noexcept { return cells.crend(); }

	LevelMap() noexcept {}
	LevelMap(const std::string_view tilFileName, const std::string_view flagsFileName,
		int32_t width_, int32_t height_, int32_t defaultTile = -1);
	LevelMap(std::weak_ptr<LevelFlags> flags_, int32_t width_, int32_t height_, int32_t defaultTile = -1);
	LevelMap(int32_t width_, int32_t height_, int32_t defaultTile = -1);

	LevelMap& operator=(const LevelMap&) = default;
	LevelMap& operator=(LevelMap&&) = default;
	LevelMap(const LevelMap&) = default;
	LevelMap(LevelMap&&) = default;

	void resize(int32_t width_, int32_t height_);

	// clears map and keeps size
	void clear(int32_t defaultTile = -1);

	void setDefaultTileSize(int32_t tileWidth_, int32_t tileHeight_, uint32_t subTiles_) noexcept;

	void setFlags(std::weak_ptr<LevelFlags> flags_) noexcept { flagsVariant = flags_; }

	uint8_t getDefaultLight() const noexcept { return defaultLight.light; }
	void setDefaultLightSource(LightSource light_) noexcept { defaultLight = light_; }

	void loadLightMap(const std::string_view fileName);
	uint8_t getLight(size_t index) const;

	void updateLights(const std::vector<std::shared_ptr<LevelObject>>& levelObjects, const sf::Vector2f& drawCenter);

	auto& AllLights() const noexcept { return allLights; }

	static auto MaxLights() noexcept { return maxLights; }
	static void MaxLights(uint32_t maxLights_) noexcept;

	// sets area (tileBlock Vector2D file) for layer 0 and uses the Flags file to set the Flags layer.
	void setTileSetAreaUseFlags(int32_t x, int32_t y, const Vector2D<int32_t>& vec);

	// sets area (indexes Vector2D file) for layer n and uses the Flags file to set the Flags layer.
	void setSimpleAreaUseFlags(size_t layer, int32_t x, int32_t y, const Vector2D<int32_t>& vec);

	// sets area (indexes Vector2D file)
	void setSimpleArea(size_t layer, int32_t x, int32_t y, const Vector2D<int32_t>& vec, bool normalizeFlagsLayer = true);

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
	void setD2Area(int32_t x, int32_t y, DS1::Decoder& dun);
#endif

	auto operator[] (int32_t x) noexcept { return Misc::Helper2D(*this, x, get); }
	auto operator[] (int32_t x) const noexcept { return Misc::Helper2D(*this, x, get); }

	auto& operator[] (const PairInt32& coord) { return get(coord.x, coord.y, *this); }
	auto& operator[] (const PairInt32& coord) const { return get(coord.x, coord.y, *this); }

	auto& operator[] (const PairFloat& coord) { return get(coord.x, coord.y, *this); }
	auto& operator[] (const PairFloat& coord) const { return get(coord.x, coord.y, *this); }

	auto& MapSizei() const noexcept { return mapSizei; }
	auto& MapSizef() const noexcept { return mapSizef; }

	auto DefaultTileWidth() const noexcept { return defaultTileWidth; }
	auto DefaultTileHeight() const noexcept { return defaultTileHeight; }
	auto DefaultBlockWidth() const noexcept { return defaultBlockWidth; }
	auto DefaultBlockHeight() const noexcept { return defaultBlockHeight; }

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
	static sf::Vector2f toDrawCoord(const T& mapPos, int32_t blockWidth, int32_t blockHeight)
	{
		return sf::Vector2f(
			(float)std::round((mapPos.x - mapPos.y) * blockWidth),
			(float)std::round((mapPos.x + mapPos.y) * blockHeight)
		);
	}

	PairFloat toMapCoord(const sf::Vector2f& drawCoord) const noexcept;
	static PairFloat toMapCoord(const sf::Vector2f& drawCoord, int32_t blockWidth, int32_t blockHeight) noexcept;

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
				return obj;
			}
		}
		return nullptr;
	}

	std::vector<PairFloat> getPath(const PairFloat& a, const PairFloat& b) const;

	std::string toCSV(bool zeroBasedIndex) const;
};
