#include "catch2/catch_test_macros.hpp"

#include "Resources/FileBytes.h"
#include "Utils/NumberVector.h"

TEST_CASE("NumberVector::constructor", "Utils")
{
	SECTION("default")
	{
		NumberVector<int> vec;

		CHECK(vec.empty() == true);
		CHECK(vec.size() == 0);
		CHECK(vec.get(0) == 0);
		CHECK(vec.get(1) == 0);
	}
	SECTION("binary file")
	{
		NumberVector<uint8_t> vec("res/numberVector.bin");

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 5);
		CHECK(vec.get(0) == '1');
		CHECK(vec.get(1) == '2');
		CHECK(vec.get(2) == '3');
		CHECK(vec.get(3) == '4');
		CHECK(vec.get(4) == '5');
		CHECK(vec.get(5) == 0);
	}
	SECTION("invalid file")
	{
		NumberVector<uint8_t> vec("res/invalidFile");

		CHECK(vec.empty() == true);
		CHECK(vec.size() == 0);
		CHECK(vec.get(0) == 0);
	}
	SECTION("binary file with max")
	{
		NumberVector<uint8_t> vec("res/numberVector.bin", 10, 2);

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 2);
		CHECK(vec.get(0) == '1');
		CHECK(vec.get(1) == '2');
		CHECK(vec.get(2) == 0);
	}
	SECTION("text file")
	{
		NumberVector<int> vec("res/numberVector.txt");

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 5);
		CHECK(vec.get(0) == 1);
		CHECK(vec.get(1) == 2);
		CHECK(vec.get(2) == -3);
		CHECK(vec.get(3) == 1000);
		CHECK(vec.get(4) == 2000);
		CHECK(vec.get(5) == 0);
	}
	SECTION("text file with default value")
	{
		NumberVector<int> vec("res/numberVector.txt", -10);

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 5);
		CHECK(vec.get(0) == 1);
		CHECK(vec.get(5) == 0);
	}
	SECTION("text file with tabs, spaces, commas and new lines default value and blank lines")
	{
		NumberVector<int> vec("res/numberVector2.txt", -10);

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 6);
		CHECK(vec.get(0) == 1);
		CHECK(vec.get(1) == 2);
		CHECK(vec.get(2) == -3);
		CHECK(vec.get(3) == 1000);
		CHECK(vec.get(4) == -10);
		CHECK(vec.get(5) == 2000);
		CHECK(vec.get(6) == 0);
	}
	SECTION("text file with max")
	{
		NumberVector<int> vec("res/numberVector.txt", -10, 2);

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 2);
		CHECK(vec.get(0) == 1);
		CHECK(vec.get(1) == 2);
		CHECK(vec.get(2) == 0);
	}
}

TEST_CASE("NumberVector::loadBinary", "Utils")
{
	SECTION("empty inputStream")
	{
		FileBytes data;
		sf::MemoryInputStream inputStream = data;

		NumberVector<uint8_t> vec;
		vec.loadBinary(inputStream);

		CHECK(vec.empty() == true);
		CHECK(vec.size() == 0);
	}
	SECTION("valid inputStream")
	{
		FileBytes data = {(std::byte)1, (std::byte)2};
		sf::MemoryInputStream inputStream = data;

		NumberVector<uint8_t> vec;
		vec.loadBinary(inputStream);

		CHECK(vec.empty() == false);
		CHECK(vec.size() == 2);
		CHECK(vec.get(0) == 1);
		CHECK(vec.get(1) == 2);
	}
}
