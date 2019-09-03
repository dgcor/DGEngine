#pragma once

#include <cstdint>
#include <string_view>
#include "Utils/Helper2D.h"
#include <vector>

class Dun
{
protected:
	std::vector<int32_t> blocks;
	size_t width{ 0 };
	size_t height{ 0 };

	void resize(size_t width_, size_t height_, int32_t defaultTile);

	friend int32_t& get(size_t x, size_t y, Dun& dun);
	friend const int32_t& get(size_t x, size_t y, const Dun& dun);

public:
	Dun() noexcept {}
	Dun(size_t width_, size_t height_, int32_t defaultTile = -1);

	Misc::Helper2D<Dun, int32_t&, size_t> operator[] (size_t x) noexcept;
	Misc::Helper2D<const Dun, const int32_t&, size_t> operator[] (size_t x) const noexcept;

	void set(size_t index, int32_t val);
	void set(size_t x, size_t y, int32_t val);

	size_t Width() const noexcept { return width; }
	size_t Height() const noexcept { return height; }
};

class D1Dun : public Dun
{
public:
	using Dun::Dun;
	D1Dun(const std::string_view fileName, int32_t defaultTile = -1);
};
