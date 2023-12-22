#include "catch2/catch_test_macros.hpp"

#include "Resources/FileBytes.h"

TEST_CASE("FileBytes::constructor", "FileBytes")
{
	SECTION("default")
	{
		FileBytes fileBytes;

		CHECK(fileBytes.empty() == true);
	}
	SECTION("size + default value")
	{
		FileBytes fileBytes(10, {});

		CHECK(fileBytes.size() == 10);
	}
}

TEST_CASE("FileBytes::MemoryInputStream", "FileBytes")
{
	SECTION("default")
	{
		FileBytes fileBytes;
		sf::MemoryInputStream inputStream = fileBytes;

		CHECK(inputStream.getSize() == -1);
	}
	SECTION("size + default value")
	{
		FileBytes fileBytes(10, {});
		sf::MemoryInputStream inputStream = fileBytes;

		CHECK(inputStream.getSize() == 10);
	}
}
