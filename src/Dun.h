#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "Helper2D.h"

class Dun
{
private:
	std::vector<int16_t> blocks;
	size_t width;
	size_t height;

	void resize(size_t width_, size_t height_);

	friend const int16_t& get(size_t x, size_t y, const Dun& dun);
	friend int16_t& get(size_t x, size_t y, Dun& dun);

public:
	Dun() {}
	Dun(const std::string&);
	Dun(size_t width_, size_t height_);

	Misc::Helper2D<Dun, int16_t&> operator[] (size_t x);
	Misc::Helper2D<const Dun, const int16_t&> operator[] (size_t x) const;

	size_t Width() const { return width; }
	size_t Height() const { return height; }
};
