#pragma once

#include <cstdint>
#include "Dun.h"
#include "Helper2D.h"
#include <SFML/Graphics/Drawable.hpp>
#include "TileSet.h"
#include "Sol.h"
#include <vector>

struct LevelCell
{
	int16_t minIndex{ -1 };
	const sf::Drawable* drawable{ nullptr };
	int8_t sol{ 0 };

	bool Passable() const { return !(sol & 0x01); }
};

class LevelMap
{
private:
	static int tileSize;

	std::vector<LevelCell> cells;
	size_t width{ 0 };
	size_t height{ 0 };
	sf::Vector2i size;

	static const LevelCell& get(size_t x, size_t y, const LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}
	static LevelCell& get(size_t x, size_t y, LevelMap& map)
	{
		return map.cells[x + y * map.Width()];
	}

public:
	LevelMap() {}
	LevelMap(size_t width_, size_t height_);

	void setArea(size_t x, size_t y, const Dun& dun, const TileSet& til, const Sol& sol);

	Misc::Helper2D<LevelMap, LevelCell&> operator[] (size_t x)
	{
		return Misc::Helper2D<LevelMap, LevelCell&>(*this, x, get);
	}
	Misc::Helper2D<const LevelMap, const LevelCell&> operator[] (size_t x) const
	{
		return Misc::Helper2D<const LevelMap, const LevelCell&>(*this, x, get);
	}

	size_t Width() const { return width; }
	size_t Height() const { return height; }

	const sf::Vector2i& Size() const { return size; }

	static int TileSize() { return tileSize; }

	sf::Vector2f getCoords(const sf::Vector2i& tile) const;
	sf::Vector2i getTile(const sf::Vector2f& coords) const;
};
