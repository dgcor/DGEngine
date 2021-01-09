#pragma once

#include <concepts>
#include "Helper2D.h"
#include <vector>

template<std::integral T>
class Vector2D;

template<std::integral T>
static T& get(size_t x, size_t y, Vector2D<T>& vec);

template<std::integral T>
static const T& get(size_t x, size_t y, const Vector2D<T>& vec);

template<std::integral T>
class Vector2D
{
protected:
	std::vector<T> elements;
	size_t width{ 0 };
	size_t height{ 0 };

	void resize(size_t width_, size_t height_, T defaultTile)
	{
		width = width_;
		height = height_;
		elements.resize(width * height, defaultTile);
	}

	friend T& get<>(size_t x, size_t y, Vector2D<T>& vec);
	friend const T& get<>(size_t x, size_t y, const Vector2D<T>& vec);

public:
	Vector2D() noexcept {}
	Vector2D(size_t width_, size_t height_, T defaultTile)
	{
		resize(width_, height_, defaultTile);
	}

	Misc::Helper2D<Vector2D, T&, size_t> operator[] (size_t x) noexcept
	{
		return Misc::Helper2D<Vector2D, T&, size_t>(*this, x, get);
	}

	Misc::Helper2D<const Vector2D, const T&, size_t> operator[] (size_t x) const noexcept
	{
		return Misc::Helper2D<const Vector2D, const T&, size_t>(*this, x, get);
	}

	void set(size_t index, T val)
	{
		if (index >= elements.size())
		{
			return;
		}
		elements[index] = std::max((T)-1, val);
	}

	void set(size_t x, size_t y, T val)
	{
		set(x + y * width, val);
	}

	size_t Width() const noexcept { return width; }
	size_t Height() const noexcept { return height; }
};


template<std::integral T>
static T& get(size_t x, size_t y, Vector2D<T>& vec)
{
	return vec.elements[x + y * vec.Width()];
}

template<std::integral T>
static const T& get(size_t x, size_t y, const Vector2D<T>& vec)
{
	return vec.elements[x + y * vec.Width()];
}
