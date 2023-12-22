#pragma once

#include <random>

class RandomGenerator
{
protected:
	static std::random_device rd;
	static std::mt19937 generator;
};

// uniform random number generator
class Random : public RandomGenerator
{
public:
	// Returns number between [0, max]
	template <class T>
	static T get(T max)
	{
		std::uniform_int_distribution<T> dist(0, max);
		return dist(generator);
	}

	// Returns number between [min, max]
	template <class T>
	static T get(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(generator);
	}

	// Returns number between [min, max]
	template <class T>
	static T getf()
	{
		std::uniform_real_distribution<T> dist(0., 1.);
		return dist(generator);
	}

	// Returns number between [0, max)
	template <class T>
	static T getf(T max)
	{
		std::uniform_real_distribution<T> dist(0., max);
		return dist(generator);
	}

	// Returns number between [0, max)
	template <class T>
	static T getf(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(generator);
	}
};

// normal distribution random number generator
class RandomNormal : public RandomGenerator
{
public:
	// returns a normal distributed number with stdDev = 1
	template <class T>
	static T get(T mean)
	{
		std::normal_distribution<T> dist(mean, (T)1);
		return dist(generator);
	}

	template <class T>
	static T get(T mean, T stdDev)
	{
		std::normal_distribution<T> dist(mean, stdDev);
		return dist(generator);
	}

	// returns a normal distributed number between min and max tending towards the mean
	template <class T>
	static T getRange(T min, T mean, T max)
	{
		T averageBoundWidth = ((mean - min) + (max - mean)) / (T)2.0;
		T standardDeviation = averageBoundWidth / (T)3.0;
		std::normal_distribution<T> distribution(mean, standardDeviation);

		T value;
		do
		{
			value = distribution(generator);
		} while (value < min || max < value);
		return value;
	}

	// returns a normal distributed number between 0 and num
	// num must be > 0
	// example: 5000 x std::round(getRange(4.0))
	// 0 - 50
	// 1 - 1094
	// 2 - 2757
	// 3 - 1047
	// 4 - 52
	template <class T>
	static T getRange(T num)
	{
		T mean = num * (T)0.5;
		T standardDeviation = mean / (T)3.0;
		std::normal_distribution<T> distribution(mean, standardDeviation);

		T value;
		do
		{
			value = distribution(generator);
		} while (value < T{} || num < value);
		return value;
	}
};
