#include "catch2/catch_test_macros.hpp"

#include "Utils/Vector2D.h"

TEST_CASE("Vector2D::constructor", "Utils")
{
	SECTION("0 elements")
	{
		Vector2D<int> vec;

		CHECK(vec.Width() == 0u);
		CHECK(vec.Height() == 0u);
		vec.set(0, 0, 10);
		CHECK(vec.Width() == 0u);
		CHECK(vec.Height() == 0u);
	}
	SECTION("2 elements")
	{
		Vector2D<int> vec(2, 2, 1);

		CHECK(vec.Width() == 2u);
		CHECK(vec.Height() == 2u);
		CHECK(vec[0][0] == 1);
		CHECK(vec[0][1] == 1);
		CHECK(vec[1][0] == 1);
		CHECK(vec[1][1] == 1);
	}
}

TEST_CASE("Vector2D::[]", "Utils")
{
	Vector2D<int> vec(2, 2, 1);

	CHECK(vec.Width() == 2u);
	CHECK(vec.Height() == 2u);
	CHECK(vec[0][0] == 1);
	CHECK(vec[0][1] == 1);
	CHECK(vec[1][0] == 1);
	CHECK(vec[1][1] == 1);

	vec[0][0] = 1;
	vec[0][1] = 2;
	vec[1][0] = 3;
	vec[1][1] = 4;

	CHECK(vec[0][0] == 1);
	CHECK(vec[0][1] == 2);
	CHECK(vec[1][0] == 3);
	CHECK(vec[1][1] == 4);

	int count = 0;
	for (size_t j = 0; j < vec.Height(); j++)
	{
		for (size_t i = 0; i < vec.Width(); i++)
		{
			CHECK(vec[j][i] == ++count);
		}
	}
}

TEST_CASE("Vector2D::set", "Utils")
{
	SECTION("set x, y")
	{
		Vector2D<int> vec(2, 2, 1);

		CHECK(vec.Width() == 2u);
		CHECK(vec.Height() == 2u);
		CHECK(vec[0][0] == 1);
		CHECK(vec[0][1] == 1);
		CHECK(vec[1][0] == 1);
		CHECK(vec[1][1] == 1);

		vec.set(0, 0, 1);
		vec.set(0, 1, 2);
		vec.set(1, 0, 3);
		vec.set(1, 1, 4);

		CHECK(vec[0][0] == 1);
		CHECK(vec[0][1] == 2);
		CHECK(vec[1][0] == 3);
		CHECK(vec[1][1] == 4);

		int count = 0;
		for (size_t j = 0; j < vec.Height(); j++)
		{
			for (size_t i = 0; i < vec.Width(); i++)
			{
				CHECK(vec[j][i] == ++count);
			}
		}
	}
	SECTION("set index")
	{
		Vector2D<int> vec(2, 2, 1);

		CHECK(vec.Width() == 2u);
		CHECK(vec.Height() == 2u);
		CHECK(vec[0][0] == 1);
		CHECK(vec[0][1] == 1);
		CHECK(vec[1][0] == 1);
		CHECK(vec[1][1] == 1);

		vec.set(0, 1);
		vec.set(1, 3);
		vec.set(2, 2);
		vec.set(3, 4);

		CHECK(vec[0][0] == 1);
		CHECK(vec[0][1] == 2);
		CHECK(vec[1][0] == 3);
		CHECK(vec[1][1] == 4);

		int count = 0;
		for (size_t j = 0; j < vec.Height(); j++)
		{
			for (size_t i = 0; i < vec.Width(); i++)
			{
				CHECK(vec[j][i] == ++count);
			}
		}
	}
}
