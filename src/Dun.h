#pragma once

#include <cstdint>
#include <string>
#include "Utils/Helper2D.h"
#include <vector>

class Dun
{
private:
	std::vector<int16_t> blocks;
	size_t width{ 0 };
	size_t height{ 0 };

	void resize(size_t width_, size_t height_, int16_t defaultTile);

	friend const int16_t& get(size_t x, size_t y, const Dun& dun);
	friend int16_t& get(size_t x, size_t y, Dun& dun);

public:
	Dun() noexcept {}
	Dun(const std::string& fileName, int16_t defaultTile = -1);
	Dun(size_t width_, size_t height_, int16_t defaultTile = -1);

	Misc::Helper2D<Dun, int16_t&, size_t> operator[] (size_t x) noexcept;
	Misc::Helper2D<const Dun, const int16_t&, size_t> operator[] (size_t x) const noexcept;

	void set(size_t index, int16_t val);
	void set(size_t x, size_t y, int16_t val);

	size_t Width() const noexcept { return width; }
	size_t Height() const noexcept { return height; }
};
