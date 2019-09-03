#include "Dun.h"
#include <algorithm>
#include "PhysFSStream.h"
#include "StreamReader.h"

D1Dun::D1Dun(const std::string_view fileName, int32_t defaultTile)
{
	sf::PhysFSStream file(fileName.data());
	if (file.hasError() == true)
	{
		return;
	}
	std::vector<uint8_t> fileData((size_t)file.getSize());
	file.read(fileData.data(), file.getSize());

	LittleEndianStreamReader fileStream(fileData.data(), fileData.size());

	width = fileStream.read<uint16_t>();
	height = fileStream.read<uint16_t>();

	blocks.resize(width * height);
	for (auto& block : blocks)
	{
		int16_t val;
		fileStream.read(val);
		block = val;
	}
	std::transform(blocks.begin(), blocks.end(), blocks.begin(), [](int32_t i) { return i - 1; });
	if (defaultTile >= 0)
	{
		std::replace(blocks.begin(), blocks.end(), -1, defaultTile);
	}
}

Dun::Dun(size_t width_, size_t height_, int32_t defaultTile)
{
	resize(width_, height_, defaultTile);
}

void Dun::resize(size_t width_, size_t height_, int32_t defaultTile)
{
	width = width_;
	height = height_;
	blocks.resize(width * height, defaultTile);
}

int32_t& get(size_t x, size_t y, Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

const int32_t& get(size_t x, size_t y, const Dun& dun)
{
	return dun.blocks[x + y * dun.Width()];
}

Misc::Helper2D<Dun, int32_t&, size_t> Dun::operator[] (size_t x) noexcept
{
	return Misc::Helper2D<Dun, int32_t&, size_t>(*this, x, get);
}

Misc::Helper2D<const Dun, const int32_t&, size_t> Dun::operator[] (size_t x) const noexcept
{
	return Misc::Helper2D<const Dun, const int32_t&, size_t>(*this, x, get);
}

void Dun::set(size_t index, int32_t val)
{
	if (index >= blocks.size())
	{
		return;
	}
	blocks[index] = std::max((int32_t)-1, val);
}

void Dun::set(size_t x, size_t y, int32_t val)
{
	set(x + y * width, val);
}
