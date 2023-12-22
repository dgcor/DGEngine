#include "catch2/catch_test_macros.hpp"

#include "Utils/PairXY.h"

TEST_CASE("PairInt32::constructor", "Utils")
{
	SECTION("default")
	{
		PairInt32 pair;

		CHECK(pair.x == 0);
		CHECK(pair.y == 0);
	}
	SECTION("valid")
	{
		PairInt32 pair(-1, 2);

		CHECK(pair.x == -1);
		CHECK(pair.y == 2);
	}
}

TEST_CASE("PairInt32::+=", "Utils")
{
	SECTION("+")
	{
		auto pair1 = PairInt32(-1, 2) + PairInt32(5, 9);

		CHECK(pair1.x == 4);
		CHECK(pair1.y == 11);
	}
	SECTION("+=")
	{
		PairInt32 pair1(-1, 2);
		PairInt32 pair2(5, 9);

		pair1 += pair2;

		CHECK(pair1.x == 4);
		CHECK(pair1.y == 11);
	}
}

TEST_CASE("PairInt32::-=", "Utils")
{
	SECTION("-")
	{
		auto pair1 = PairInt32(-1, 2) - PairInt32(5, 9);

		CHECK(pair1.x == -6);
		CHECK(pair1.y == -7);
	}
	SECTION("-=")
	{
		PairInt32 pair1(-1, 2);
		PairInt32 pair2(5, 9);

		pair1 -= pair2;

		CHECK(pair1.x == -6);
		CHECK(pair1.y == -7);
	}
}

TEST_CASE("PairInt32::*=", "Utils")
{
	SECTION("*")
	{
		auto pair = PairInt32(-1, 2) * 2;

		CHECK(pair.x == -2);
		CHECK(pair.y == 4);
	}
	SECTION("*=")
	{
		PairInt32 pair(-1, 2);

		pair *= 2;

		CHECK(pair.x == -2);
		CHECK(pair.y == 4);
	}
}

TEST_CASE("PairInt32::/=", "Utils")
{
	SECTION("/")
	{
		auto pair = PairInt32(-1, 2) / 2;

		CHECK(pair.x == 0);
		CHECK(pair.y == 1);
	}
	SECTION("/=")
	{
		PairInt32 pair(-1, 2);

		pair /= 2;

		CHECK(pair.x == 0);
		CHECK(pair.y == 1);
	}
}

TEST_CASE("PairInt32::<=>", "Utils")
{
	SECTION("==")
	{
		PairInt32 pair1(2, 2);
		PairInt32 pair2(2, 2);

		CHECK(pair1 == pair2);
		CHECK((pair1 != pair2) == false);
	}
	SECTION("!=")
	{
		PairInt32 pair1(2, 2);
		PairInt32 pair2(1, 3);

		CHECK(pair1 != pair2);
		CHECK((pair1 == pair2) == false);
	}
	SECTION("<")
	{
		PairInt32 pair1(-1, 2);
		PairInt32 pair2(5, 9);

		CHECK(pair1 < pair2);
		CHECK((pair1 > pair2 == false));

		PairInt32 pair3(-1, 2);
		PairInt32 pair4(5, -5);

		CHECK(pair3 < pair4);
		CHECK((pair3 > pair4 == false));
	}
}
