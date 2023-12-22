#include "catch2/catch_test_macros.hpp"

#include "Utils/FixedArray.h"

TEST_CASE("FixedArray::constructor", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.max_size() == 0u);
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.max_size() == 2u);
	}
}

TEST_CASE("FixedArray::iterators", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		for (auto& val : arr)
		{
			CHECK(val == 10000000);
		}
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.push_back(100) == true);
		CHECK(arr.push_back(200) == true);

		for (auto& val : arr)
		{
			CHECK((val == 100 || val == 200));
		}
		for (const auto& val : arr)
		{
			CHECK((val == 100 || val == 200));
		}
	}
}

TEST_CASE("FixedArray::clear", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
		arr.clear();
		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
		CHECK(arr.push_back(100) == true);
		CHECK(arr.push_back(200) == true);
		CHECK(arr.size() == 2u);
		CHECK(arr.empty() == false);
		arr.clear();
		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
	}
}

TEST_CASE("FixedArray::data", "Utils")
{
	FixedArray<int, 2> arr;

	CHECK(arr.push_back(100) == true);
	CHECK(arr.push_back(200) == true);
	CHECK(*arr.data() == 100);
	CHECK(arr.data()[1] == 200);
}

TEST_CASE("FixedArray::empty", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
		CHECK(arr.push_back(100) == false);
		CHECK(arr.empty() == true);
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.empty() == true);
		CHECK(arr.push_back(100) == true);
		CHECK(arr.size() == 1u);
		CHECK(arr.empty() == false);
	}
}

TEST_CASE("FixedArray::full", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.full() == true);
		CHECK(arr.push_back(100) == false);
		CHECK(arr.full() == true);
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.full() == false);
		CHECK(arr.push_back(100) == true);
		CHECK(arr.size() == 1u);
		CHECK(arr.full() == false);
		CHECK(arr.push_back(200) == true);
		CHECK(arr.size() == 2u);
		CHECK(arr.full() == true);
		CHECK(arr.push_back(300) == false);
		CHECK(arr.size() == 2u);
		CHECK(arr.full() == true);
	}
}

TEST_CASE("FixedArray::[]", "Utils")
{
	FixedArray<int, 2> arr;

	CHECK(arr.size() == 0u);
	CHECK(arr.max_size() == 2u);
	CHECK(arr.push_back(100) == true);
	CHECK(arr.push_back(200) == true);
	CHECK(arr.size() == 2u);

	CHECK(arr[0] == 100);
	CHECK(arr[1] == 200);

	arr[0] = 300;
	arr[1] = 400;

	CHECK(arr[0] == 300);
	CHECK(arr[1] == 400);
}

TEST_CASE("FixedArray::push_back", "Utils")
{
	SECTION("0 elements")
	{
		FixedArray<int, 0> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.max_size() == 0u);
		CHECK(arr.push_back(100) == false);
		CHECK(arr.size() == 0u);
	}
	SECTION("2 elements")
	{
		FixedArray<int, 2> arr;

		CHECK(arr.size() == 0u);
		CHECK(arr.max_size() == 2u);
		CHECK(arr.push_back(100) == true);
		CHECK(arr.push_back(200) == true);
		CHECK(arr.push_back(300) == false);
		CHECK(arr.size() == 2u);
	}
}
