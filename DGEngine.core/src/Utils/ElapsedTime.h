#pragma once

#include <cstdint>
#include <SFML/System/Time.hpp>
#include <type_traits>

template <class T>
class ElapsedTimeT
{
public:
	T timeout{};
	T currentTime{};

	constexpr ElapsedTimeT(T timeout_ = {}) noexcept : timeout(timeout_) {}

	constexpr void reset() noexcept { currentTime = {}; }

	// reset time, and keep the remainder
	// if T is float, this function is the same as updateAndReset
	constexpr bool update(T elapsedTime) noexcept
	{
		if constexpr (std::is_floating_point_v<T> == true)
		{
			return updateAndReset(elapsedTime);
		}
		else
		{
			if (timeout == T{})
			{
				return true;
			}
			currentTime += elapsedTime;
			if (currentTime >= timeout)
			{
				currentTime = currentTime % timeout;
				return true;
			}
			return false;
		}
	}

	// reset time, and reset the remainder
	constexpr bool updateAndReset(T elapsedTime) noexcept
	{
		currentTime += elapsedTime;
		if (currentTime >= timeout)
		{
			currentTime = {};
			return true;
		}
		return false;
	}

	// reset time, and subtract the remainder
	constexpr bool updateAndSubtract(T elapsedTime) noexcept
	{
		currentTime += elapsedTime;
		if (currentTime >= timeout)
		{
			currentTime -= timeout;
			return true;
		}
		return false;
	}

	// execute function on each timeout while currentTime >= timeout
	// and subtract timeout on each iteration
	// or until function returns false if the function returns a bool
	template<typename Functor>
	constexpr void update(T elapsedTime, Functor&& func) noexcept
	{
		currentTime += elapsedTime;
		while (currentTime >= timeout)
		{
			currentTime -= timeout;
			if constexpr (std::is_same_v<std::invoke_result_t<Functor>, bool> == true)
			{
				if (func() == false)
				{
					break;
				}
			}
			else
			{
				func();
			}
		}
	}
};

using ElapsedTime = ElapsedTimeT<sf::Time>;
using ElapsedTimef = ElapsedTimeT<float>;
