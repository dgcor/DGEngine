#pragma once

#include "EasingFunctions.h"

template<typename T>
using EasingFunctionPtr = T (*)(T, T, T, T);

template <class T, EasingFunctionPtr<T> EasingFunc = EasingFunctions::easeOutQuart>
class EasedValue
{
private:
	T startValue{};	// current value, if easing is disabled/finished
	T diffValue{};
	T currentTime{};
	T stopTime{};

	bool easingEnabled() const noexcept { return stopTime > T{}; }

	// false if easing is disabled
	bool hasNewValue() const noexcept { return currentTime < stopTime; }

	T getEased() const noexcept
	{
		return EasingFunc(
			currentTime,
			startValue,
			diffValue,
			stopTime);
	}

	T getEasedFinal() const noexcept
	{
		return EasingFunc(
			stopTime,
			startValue,
			diffValue,
			stopTime);
	}

public:
	EasedValue(T value_ = {}) noexcept : startValue(value_) {}

	// Uses current value as starting value if easing is enabled
	// if duration <= 0, easing is disabled
	void set(T value_, T stopTime_) noexcept
	{
		if (stopTime_ <= T{})
		{
			// if easing disabled
			stopTime = stopTime_;
			startValue = value_;
			currentTime = stopTime_;
		}
		else
		{
			// if easing enabled
			if (hasNewValue() == true)
			{
				startValue = get();
			}
			stopTime = stopTime_;
			diffValue = value_ - startValue;
			currentTime = {};
		}
	}

	// current value
	T get() const noexcept
	{
		if (hasNewValue() == false)
		{
			return startValue;
		}
		else
		{
			return getEased();
		}
	}

	// final value
	T getFinal() const noexcept
	{
		if (hasNewValue() == false)
		{
			return startValue;
		}
		else
		{
			return getEasedFinal();
		}
	}

	void update(T elapsedTime) noexcept
	{
		if (hasNewValue() == false)
		{
			return;
		}
		if (easingEnabled() == true)
		{
			currentTime += elapsedTime;
			if (currentTime >= stopTime)
			{
				currentTime = stopTime;
				startValue = getEasedFinal();
			}
		}
	}
};

using EasedValuef = EasedValue<float, EasingFunctions::easeOutQuart>;
using EasedValued = EasedValue<double, EasingFunctions::easeOutQuart>;
