#pragma once

#include <cstdlib>

namespace Misc
{
	///
	/// Class to help turn one dimensional storage into 2D, intended to be used as the
	/// return value of an operator[] method, to allow object.[x][y] style addressing
	///
	template <class From, class Retval> class Helper2D
	{
	private:
		From& parent;
		size_t x;
		Retval&(*func)(size_t, size_t, From&);

	public:
		Helper2D(From& parent_, size_t x_, Retval(*func_)(size_t, size_t, From&))
			: parent(parent_), x(x_), func(func_) {}

		Retval& operator[](size_t y) { return func(x, y, parent); }
	};
}
