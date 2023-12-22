#pragma once

#include <functional>

// binary search function to find the element using a search function
// returns searchFunc's return value if found or -1 if not found
template <class T = int>
T BinarySearch(T start, T stop, T search, const std::function<T(T, T)> searchFunc)
{
	static_assert(std::is_integral_v<T>, "integer values only");

	while (start <= stop)
	{
		T middle = start + (stop - start) / 2;

		auto foundValue = searchFunc(middle, search);

		if (foundValue == search)
		{
			return middle;
		}
		if (foundValue < search)
		{
			start = middle + 1;
		}
		else
		{
			stop = middle - 1;
		}
	}
	return -1;
}
