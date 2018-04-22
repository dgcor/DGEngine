#include "Dun.h"
#include <algorithm>
#include "PhysFSStream.h"
#include "StreamReader.h"

Dun::Dun(const std::string& fileName, int16_t defaultTile)
{
	sf::PhysFSStream file(fileName);
	if (file.hasError() == true)
	{
		return;
	}
	std::vector<uint8_t> fileData((size_t)file.getSize());
	file.read(fileData.data(), file.getSize());

	LittleEndianStreamReader fileStream(fileData);

	width = fileStream.read<uint16_t>();
	height = fileStream.read<uint16_t>();

	blocks.resize(width * height);
	for (auto& block : blocks)
	{
		fileStream.read(block);
	}
	std::transform(blocks.begin(), blocks.end(), blocks.begin(), [](int16_t i) { return i - 1; });
	if (defaultTile >= 0)
	{
		std::replace(blocks.begin(), blocks.end(), (int16_t)-1, defaultTile);
	}
}

Dun::Dun(size_t width_, size_t height_, int16_t defaultTile)
{
	resize(width_, height_, defaultTile);
}

void Dun::resize(size_t width_, size_t height_, int16_t defaultTile)
{
	width = width_;
	height = height_;
	blocks.resize(width * height, defaultTile);
}

int16_t& get(size_t x, size_t y, Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

const int16_t& get(size_t x, size_t y, const Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

Misc::Helper2D<Dun, int16_t&, size_t> Dun::operator[] (size_t x) noexcept
{
	return Misc::Helper2D<Dun, int16_t&, size_t>(*this, x, get);
}

Misc::Helper2D<const Dun, const int16_t&, size_t> Dun::operator[] (size_t x) const noexcept
{
	return Misc::Helper2D<const Dun, const int16_t&, size_t>(*this, x, get);
}

void Dun::set(size_t index, int16_t val)
{
	if (index >= blocks.size())
	{
		return;
	}
	blocks[index] = std::max((int16_t)-1, val);
}

void Dun::set(size_t x, size_t y, int16_t val)
{
	set(x + y * width, val);
}
