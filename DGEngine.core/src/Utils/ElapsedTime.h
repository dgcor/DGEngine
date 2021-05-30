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
	constexpr bool update(T elapsedTime) noexcept
	{
		if constexpr (std::is_floating_point_v<T> == true)
		{
			return updateAndReset(elapsedTime);
		}
		else
		{
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
			using FunctorReturnType = std::invoke_result<Functor, void>;
			if constexpr (std::is_same<FunctorReturnType, bool>::value == true)
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

#define ElapsedTime ElapsedTimeT<sf::Time>
#define ElapsedTimef ElapsedTimeT<float>
