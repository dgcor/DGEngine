#include "catch2/catch_test_macros.hpp"

#include "Utils/FixedMap.h"

TEST_CASE("FixedMap::constructor", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 0u);
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 2u);
	}
}

TEST_CASE("FixedMap::iterators", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		for (auto& val : map)
		{
			CHECK(val.first == "invalid");
			CHECK(val.second == 10000000);
		}
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == true);

		for (auto& val : map)
		{
			CHECK((val.first == "key1" || val.first == "key2"));
			CHECK((val.second == 100 || val.second == 200));
		}
		for (const auto& val : map)
		{
			CHECK((val.first == "key1" || val.first == "key2"));
			CHECK((val.second == 100 || val.second == 200));
		}
	}
}

TEST_CASE("FixedMap::empty", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.size() == 0u);
		CHECK(map.empty() == true);
		CHECK(map.setValue("key1", 100) == false);
		CHECK(map.empty() == true);
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.empty() == true);
		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.size() == 1u);
		CHECK(map.empty() == false);
		map.deleteValue("key1");
		CHECK(map.size() == 0u);
		CHECK(map.empty() == true);
	}
}

TEST_CASE("FixedMap::hasValue", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.setValue("key1", 100) == false);
		CHECK(map.hasValue("key1") == false);
	}
	SECTION("1 element")
	{
		FixedMap<std::string, int, 1> map;

		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == false);
		CHECK(map.hasValue("key1") == true);
		CHECK(map.hasValue("key2") == false);
		CHECK(map.hasValue("key3") == false);
		map.deleteValue("key1");
		CHECK(map.hasValue("key1") == false);
	}
}

TEST_CASE("FixedMap::getValue", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.setValue("key1", 100) == false);
		CHECK(map.size() == 0u);

		int val = 0;
		CHECK(map.getValue("key1", val) == false);
		CHECK(map.getValue("key1") == nullptr);
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 2u);
		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == true);
		CHECK(map.size() == 2u);

		int val = 0;
		CHECK(map.getValue("key1", val) == true);
		CHECK(val == 100);
		int* valPtr = map.getValue("key1");
		CHECK((valPtr != nullptr && *valPtr == 100));

		int val2 = 0;
		CHECK(map.getValue("key2", val2) == true);
		CHECK(val2 == 200);
		int* val2Ptr = map.getValue("key2");
		CHECK((val2Ptr != nullptr && *val2Ptr == 200));

		int val3 = 0;
		CHECK(map.getValue("key3", val2) == false);
		CHECK(val3 == 0);
		CHECK(map.getValue("key3") == nullptr);

		map.deleteValue("key1");

		val = 0;
		CHECK(map.getValue("key1", val) == false);
		CHECK(val == 0);
		CHECK(map.getValue("key1") == nullptr);

		val2 = 0;
		CHECK(map.getValue("key2", val2) == true);
		CHECK(val2 == 200);
		val2Ptr = map.getValue("key2");
		CHECK((val2Ptr != nullptr && *val2Ptr == 200));

		map.deleteValue("key2");

		val = 0;
		CHECK(map.getValue("key1", val) == false);
		CHECK(val == 0);
		CHECK(map.getValue("key1") == nullptr);

		val2 = 0;
		CHECK(map.getValue("key2", val2) == false);
		CHECK(val2 == 0);
		CHECK(map.getValue("key2") == nullptr);
	}
}

TEST_CASE("FixedMap::setValue", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 0u);
		CHECK(map.setValue("key1", 100) == false);
		CHECK(map.size() == 0u);
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 2u);
		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == true);
		CHECK(map.setValue("key3", 300) == false);
		CHECK(map.size() == 2u);
	}
	SECTION("2 elements same key")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 2u);
		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == true);
		CHECK(map.setValue("key1", 300) == true);
		CHECK(map.size() == 2u);
	}
}

TEST_CASE("FixedMap::deleteValue", "Utils")
{
	SECTION("0 elements")
	{
		FixedMap<std::string, int, 0> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 0u);
		map.deleteValue("key1");
		CHECK(map.size() == 0u);
	}
	SECTION("2 elements")
	{
		FixedMap<std::string, int, 2> map;

		CHECK(map.size() == 0u);
		CHECK(map.max_size() == 2u);
		CHECK(map.setValue("key1", 100) == true);
		CHECK(map.setValue("key2", 200) == true);
		CHECK(map.size() == 2u);
		map.deleteValue("key3");
		CHECK(map.size() == 2u);
		map.deleteValue("key1");
		CHECK(map.hasValue("key2") == true);
		CHECK(map.size() == 1u);
		map.deleteValue("key1");
		CHECK(map.hasValue("key2") == true);
		CHECK(map.size() == 1u);
		map.deleteValue("key2");
		CHECK(map.size() == 0u);
		CHECK(map.hasValue("key2") == false);
	}
}
