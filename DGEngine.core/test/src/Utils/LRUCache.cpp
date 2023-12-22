#include "catch2/catch_test_macros.hpp"

#define private public
#define protected public

#include "Utils/LRUCache.h"

TEST_CASE("LRUCache::getValue", "Utils")
{
	LRUCache<std::string, int, 3> cache;

	CHECK(cache.size() == 0u);
	CHECK(cache.max_size() == 3u);
	cache.updateValue("key3", 300);
	cache.updateValue("key2", 200);
	cache.updateValue("key1", 100);
	CHECK(cache.size() == 3u);

	auto count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			case 1:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 2:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			default:
				break;
		}
	}

	int val = 0;
	CHECK(cache.getValue("key3", val) == true);
	CHECK(val == 300);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			case 1:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			case 2:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			default:
				break;
		}
	}

	int* valPtr = cache.getValue("key2");
	CHECK((valPtr != nullptr && *valPtr == 200));

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 1:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			case 2:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			default:
				break;
		}
	}

	val = 0;
	CHECK(cache.getValue("key2", val) == true);
	CHECK(val == 200);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 1:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			case 2:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			default:
				break;
		}
	}
}

TEST_CASE("LRUCache::updateValue", "Utils")
{
	LRUCache<std::string, int, 3> cache;

	CHECK(cache.size() == 0u);
	CHECK(cache.max_size() == 3u);
	cache.updateValue("key3", 300);
	CHECK(cache.size() == 1u);

	auto count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key2", 200);
	CHECK(cache.size() == 2u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 1:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key1", 100);
	CHECK(cache.size() == 3u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			case 1:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 2:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key2", 2000);
	CHECK(cache.size() == 3u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key2" && val.second == 2000));
				break;
			case 1:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			case 2:
				CHECK((val.first == "key3" && val.second == 300));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key4", 400);
	CHECK(cache.size() == 3u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key4" && val.second == 400));
				break;
			case 1:
				CHECK((val.first == "key2" && val.second == 2000));
				break;
			case 2:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key2", 200);
	CHECK(cache.size() == 3u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 1:
				CHECK((val.first == "key4" && val.second == 400));
				break;
			case 2:
				CHECK((val.first == "key1" && val.second == 100));
				break;
			default:
				break;
		}
	}

	cache.updateValue("key5", 500);
	CHECK(cache.size() == 3u);

	count = 0;
	for (const auto& val : cache)
	{
		switch(count++)
		{
			case 0:
				CHECK((val.first == "key5" && val.second == 500));
				break;
			case 1:
				CHECK((val.first == "key2" && val.second == 200));
				break;
			case 2:
				CHECK((val.first == "key4" && val.second == 400));
				break;
			default:
				break;
		}
	}
}
