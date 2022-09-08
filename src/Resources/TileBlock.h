#pragma once

#include <cstdint>
#include <tuple>

class TileBlock : public std::tuple<int16_t, int16_t, int16_t, int16_t>
{
private:
	using TileBlock_t = std::tuple<int16_t, int16_t, int16_t, int16_t>;

public:
	constexpr TileBlock() : std::tuple<int16_t, int16_t, int16_t, int16_t>(-1, -1, -1, -1) {}
	constexpr TileBlock(int16_t tile) : std::tuple<int16_t, int16_t, int16_t, int16_t>(tile, tile, tile, tile) {}

	constexpr TileBlock& operator=(const TileBlock&) = default;
	constexpr TileBlock& operator=(TileBlock&&) = default;
	constexpr TileBlock(const TileBlock&) = default;
	constexpr TileBlock(TileBlock&&) = default;

	template <size_t T>
	constexpr auto& get() noexcept
	{
		return std::get<T>(*((TileBlock_t* const)this));
	}

	template <size_t T>
	constexpr const auto& get() const noexcept
	{
		return std::get<T>(*((const TileBlock_t* const)this));
	}

	constexpr int16_t getTileIndex(int32_t xCoord, int32_t yCoord) const noexcept
	{
		if (xCoord % 2)
		{
			if (yCoord % 2)
				return get<3>();
			else
				return get<1>();
		}
		else
		{
			if (yCoord % 2)
				return get<2>();
			else
				return get<0>();
		}
	}
};
