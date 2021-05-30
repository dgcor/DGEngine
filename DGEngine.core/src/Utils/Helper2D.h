#pragma once

namespace Misc
{
	///
	/// Class to help turn one dimensional storage into 2D, intended to be used as the
	/// return value of an operator[] method, to allow object.[x][y] style addressing
	///
	template <class From, class Retval, class Idx>
	class Helper2D
	{
	private:
		From& parent;
		Idx x;
		Retval&(*func)(Idx, Idx, From&);

	public:
		Helper2D(From& parent_, Idx x_, Retval(*func_)(Idx, Idx, From&)) noexcept
			: parent(parent_), x(x_), func(func_) {}

		Retval& operator[](Idx y) noexcept { return func(x, y, parent); }
	};
}
