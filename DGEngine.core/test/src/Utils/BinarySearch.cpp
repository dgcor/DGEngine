#include "catch2/catch_test_macros.hpp"

#include "Utils/BinarySearch.h"

TEST_CASE("BinarySearch", "BinarySearch")
{
	int iterations = 0;
	int keyToFind = 0;

	auto searchFunc = [&](int key, int val) -> int
	{
		iterations++;

		if (key == keyToFind)
		{
			return val;
		}
		if (key > keyToFind)
		{
			return val + 1;
		}
		else
		{
			return val - 1;
		}
	};

	SECTION("[0, 0] -> value in key 0")
	{
		iterations = 0;
		keyToFind = 0;

		auto result = BinarySearch<int>(0, 0, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 1);
	}
	SECTION("[0, 0] -> value in key 1")
	{
		iterations = 0;
		keyToFind = 1;

		auto result = BinarySearch<int>(0, 0, 2, searchFunc);

		CHECK(result == -1);
		CHECK(iterations == 1);
	}

	SECTION("[0, 1] -> value in key 0")
	{
		iterations = 0;
		keyToFind = 0;

		auto result = BinarySearch<int>(0, 1, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 1);
	}
	SECTION("[0, 1] -> value in key 1")
	{
		iterations = 0;
		keyToFind = 1;

		auto result = BinarySearch<int>(0, 1, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 2);
	}
	SECTION("[0, 1] -> value in key 2")
	{
		iterations = 0;
		keyToFind = 2;

		auto result = BinarySearch<int>(0, 1, 2, searchFunc);

		CHECK(result == -1);
		CHECK(iterations == 2);
	}

	SECTION("[0, 10] -> value in key 10")
	{
		iterations = 0;
		keyToFind = 10;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 4);
	}
	SECTION("[0, 10] -> value in key 9")
	{
		iterations = 0;
		keyToFind = 9;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 3);
	}
	SECTION("[0, 10] -> value in key 8")
	{
		iterations = 0;
		keyToFind = 8;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 2);
	}
	SECTION("[0, 10] -> value in key 7")
	{
		iterations = 0;
		keyToFind = 7;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 4);
	}
	SECTION("[0, 10] -> value in key 6")
	{
		iterations = 0;
		keyToFind = 6;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 3);
	}
	SECTION("[0, 10] -> value in key 5")
	{
		iterations = 0;
		keyToFind = 5;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 1);
	}
	SECTION("[0, 10] -> value in key 4")
	{
		iterations = 0;
		keyToFind = 4;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 4);
	}
	SECTION("[0, 10] -> value in key 3")
	{
		iterations = 0;
		keyToFind = 3;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 3);
	}
	SECTION("[0, 10] -> value in key 2")
	{
		iterations = 0;
		keyToFind = 2;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 2);
	}
	SECTION("[0, 10] -> value in key 1")
	{
		iterations = 0;
		keyToFind = 1;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 4);
	}
	SECTION("[0, 10] -> value in key 0")
	{
		iterations = 0;
		keyToFind = 0;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 3);
	}
	SECTION("[0, 10] -> value in key -5")
	{
		iterations = 0;
		keyToFind = -5;

		auto result = BinarySearch<int>(0, 10, 2, searchFunc);

		CHECK(result == -1);
		CHECK(iterations == 3);
	}

	SECTION("[0, 1000] -> value in key 1")
	{
		iterations = 0;
		keyToFind = 1;

		auto result = BinarySearch<int>(0, 1000, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 10);
	}
	SECTION("[0, 1000] -> value in key 100")
	{
		iterations = 0;
		keyToFind = 100;

		auto result = BinarySearch<int>(0, 1000, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 7);
	}
	SECTION("[0, 1000] -> value in key 800")
	{
		iterations = 0;
		keyToFind = 800;

		auto result = BinarySearch<int>(0, 1000, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 8);
	}
	SECTION("[0, 1000] -> value in key 1000")
	{
		iterations = 0;
		keyToFind = 1000;

		auto result = BinarySearch<int>(0, 1000, 2, searchFunc);

		CHECK(result == keyToFind);
		CHECK(iterations == 10);
	}
}
