#include "Dun.h"
#include <cstdio>
#include <iostream>
#include "PhysFSStream.h"

Dun::Dun(const std::string& filename)
{
	sf::PhysFSStream file(filename);

	if (file.hasError() == true)
	{
		return;
	}

	int16_t temp;
	file.read(&temp, 2);
	width = temp;
	file.read(&temp, 2);
	height = temp;

	blocks.resize(width * height);
	file.read(&blocks[0], 2 * width * height);
}

Dun::Dun(size_t width_, size_t height_)
{
	resize(width_, height_);
}

void Dun::resize(size_t width_, size_t height_)
{
	width = width_;
	height = height_;
	blocks.resize(width * height, 0);
}

int16_t& get(size_t x, size_t y, Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

const int16_t& get(size_t x, size_t y, const Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

Misc::Helper2D<Dun, int16_t&> Dun::operator[] (size_t x)
{
	return Misc::Helper2D<Dun, int16_t&>(*this, x, get);
}

Misc::Helper2D<const Dun, const int16_t&> Dun::operator[] (size_t x) const
{
	return Misc::Helper2D<const Dun, const int16_t&>(*this, x, get);
}
