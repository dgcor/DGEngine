#include "catch2/catch_test_macros.hpp"

#include <unordered_map>
#include "Utils/Random.h"

TEST_CASE("Random::get", "Utils")
{
	SECTION("[0, n]")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 100; i++)
		{
			++hist[Random::get(4)];
		}

		CHECK(hist.contains(-1) == false);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(4) == true);
		CHECK(hist.contains(5) == false);

		CHECK(hist[0] > 2);
		CHECK(hist[1] > 2);
		CHECK(hist[2] > 2);
		CHECK(hist[3] > 2);
		CHECK(hist[4] > 2);
	}
	SECTION("[n1, n2]")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 100; i++)
		{
			++hist[Random::get(-2, 2)];
		}

		CHECK(hist.contains(-3) == false);
		CHECK(hist.contains(-2) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == false);

		CHECK(hist[-2] > 2);
		CHECK(hist[-1] > 2);
		CHECK(hist[0] > 2);
		CHECK(hist[1] > 2);
		CHECK(hist[2] > 2);
	}
}

TEST_CASE("Random::getf", "Utils")
{
	SECTION("[0, 1)")
	{
		for (auto i = 0; i < 10; i++)
		{
			auto val = Random::getf<float>();
			CHECK(val >= 0.f);
			CHECK(val < 1.f);
		}
	}
	SECTION("[0, n)")
	{
		for (auto i = 0; i < 10; i++)
		{
			auto val = Random::getf(4.f);
			CHECK(val >= 0.f);
			CHECK(val < 4.f);
		}
	}
	SECTION("[n1, n2)")
	{
		for (auto i = 0; i < 10; i++)
		{
			auto val = Random::getf(-2.f, 2.f);
			CHECK(val >= -2.f);
			CHECK(val < 2.f);
		}
	}
}

TEST_CASE("RandomNormal::get", "Utils")
{
	SECTION("mean, stdDev = 1")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(RandomNormal::get(5.f))];
		}

		CHECK(hist.contains(-1) == false);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == true);
		CHECK(hist.contains(4) == true);
		CHECK(hist.contains(5) == true);
		CHECK(hist.contains(6) == true);
		CHECK(hist.contains(7) == true);
		CHECK(hist.contains(8) == true);
		CHECK(hist.contains(11) == false);

		CHECK(hist[2] < hist[3]);
		CHECK(hist[3] < hist[4]);
		CHECK(hist[4] < hist[5]);
		CHECK(hist[5] > hist[6]);
		CHECK(hist[6] > hist[7]);
		CHECK(hist[7] > hist[8]);
	}
	SECTION("mean, stdDev")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(RandomNormal::get(0.f, 2.f))];
		}

		CHECK(hist.contains(-3) == true);
		CHECK(hist.contains(-2) == true);
		CHECK(hist.contains(-1) == true);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(1) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == true);

		CHECK(hist[-3] < hist[-2]);
		CHECK(hist[-3] < hist[-1]);
		CHECK(hist[-2] < hist[0]);
		CHECK(hist[0] > hist[2]);
		CHECK(hist[1] > hist[3]);
		CHECK(hist[2] > hist[3]);
	}
}

TEST_CASE("RandomNormal::getRange", "Utils")
{
	SECTION("min, mean, max")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(RandomNormal::getRange(-2.f, 0.f, 2.f))];
		}

		CHECK(hist.size() == 5);

		CHECK(hist.contains(-3) == false);
		CHECK(hist.contains(-2) == true);
		CHECK(hist.contains(-1) == true);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(1) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == false);

		CHECK(hist[-2] < hist[-1]);
		CHECK(hist[-1] < hist[0]);
		CHECK(hist[0] > hist[1]);
		CHECK(hist[1] > hist[2]);
	}
	SECTION("min, mean == max")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(RandomNormal::getRange(-2.f, 2.f, 2.f))];
		}

		CHECK(hist.size() <= 5);

		CHECK(hist.contains(-3) == false);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(1) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == false);

		CHECK(hist[0] < hist[1]);
		CHECK(hist[1] < hist[2]);
	}
	SECTION("[0, n]")
	{
		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(RandomNormal::getRange(4.f))];
		}

		CHECK(hist.size() == 5);

		CHECK(hist.contains(-1) == false);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(1) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == true);
		CHECK(hist.contains(4) == true);
		CHECK(hist.contains(5) == false);

		CHECK(hist[0] < hist[1]);
		CHECK(hist[1] < hist[2]);
		CHECK(hist[2] > hist[3]);
		CHECK(hist[3] > hist[4]);
	}
}
