#include "catch2/catch_test_macros.hpp"

#include "Utils/ElapsedTime.h"

TEST_CASE("ElapsedTime::constructor", "Utils")
{
	SECTION("default")
	{
		ElapsedTime val;

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::Time::Zero);
	}
	SECTION("default float")
	{
		ElapsedTimef val;

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 0.f);
	}
	SECTION("timeout")
	{
		ElapsedTime val(sf::seconds(1.f));

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("timeout float")
	{
		ElapsedTimef val(1.f);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);
	}
}

TEST_CASE("ElapsedTime::reset", "Utils")
{
	SECTION("time")
	{
		ElapsedTime val(sf::seconds(1.f));

		val.update(sf::seconds(0.5f));

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.reset();

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float")
	{
		ElapsedTimef val(1.f);

		val.update(0.5f);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		val.reset();

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);
	}
}

TEST_CASE("ElapsedTime::update", "Utils")
{
	SECTION("default time")
	{
		ElapsedTime val;

		CHECK(val.update(sf::seconds(0.0)) == true);

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::Time::Zero);

		CHECK(val.update(sf::seconds(0.5f)) == true);

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::Time::Zero);
	}
	SECTION("default float")
	{
		ElapsedTimef val;

		CHECK(val.update(0.0) == true);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 0.f);

		CHECK(val.update(0.5) == true);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 0.f);
	}
	SECTION("time")
	{
		ElapsedTime val(sf::seconds(1.f));

		CHECK(val.update(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.update(sf::seconds(0.5f)) == true);

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.update(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.update(sf::seconds(1.f)) == true);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float")
	{
		ElapsedTimef val(1.f);

		CHECK(val.update(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.update(0.5f) == true);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);

		CHECK(val.update(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.update(1.f) == true);

		CHECK(val.currentTime == 0.0f);
		CHECK(val.timeout == 1.f);
	}
}

TEST_CASE("ElapsedTime::updateAndReset", "Utils")
{
	SECTION("time")
	{
		ElapsedTime val(sf::seconds(1.f));

		CHECK(val.updateAndReset(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndReset(sf::seconds(0.5f)) == true);

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndReset(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndReset(sf::seconds(1.f)) == true);

		CHECK(val.currentTime == sf::seconds(0.0));
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float")
	{
		ElapsedTimef val(1.f);

		CHECK(val.updateAndReset(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndReset(0.5f) == true);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndReset(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndReset(1.f) == true);

		CHECK(val.currentTime == 0.0f);
		CHECK(val.timeout == 1.f);
	}
}

TEST_CASE("ElapsedTime::updateAndSubtract", "Utils")
{
	SECTION("time")
	{
		ElapsedTime val(sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::seconds(0.5f)) == true);

		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::seconds(0.5f)) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::seconds(1.f)) == true);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::seconds(2.f)) == true);

		CHECK(val.currentTime == sf::seconds(1.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::Time::Zero) == true);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		CHECK(val.updateAndSubtract(sf::Time::Zero) == false);

		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float")
	{
		ElapsedTimef val(1.f);

		CHECK(val.updateAndSubtract(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(0.5f) == true);

		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(0.5f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(1.f) == true);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(2.f) == true);

		CHECK(val.currentTime == 1.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(0.f) == true);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		CHECK(val.updateAndSubtract(0.f) == false);

		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);
	}
}

TEST_CASE("ElapsedTime::updateFunc", "Utils")
{
	SECTION("time")
	{
		ElapsedTime val(sf::seconds(1.f));

		auto count = 0;

		val.update(sf::seconds(3.5f), [&]()
		{
			count++;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.update(sf::seconds(0.25f), [&]()
		{
			count++;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == sf::seconds(0.75f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.update(sf::seconds(0.25f), [&]()
		{
			count++;
		});

		CHECK(count == 4);
		CHECK(val.currentTime == sf::Time::Zero);
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float")
	{
		ElapsedTimef val(1.f);

		auto count = 0;

		val.update(3.5f, [&]()
		{
			count++;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		val.update(0.25f, [&]()
		{
			count++;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == 0.75f);
		CHECK(val.timeout == 1.f);

		val.update(0.25f, [&]()
		{
			count++;
		});

		CHECK(count == 4);
		CHECK(val.currentTime == 0.f);
		CHECK(val.timeout == 1.f);
	}
	SECTION("time and func returns bool")
	{
		ElapsedTime val(sf::seconds(1.f));

		auto count = 0;

		val.update(sf::seconds(3.5f), [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == sf::seconds(0.5f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.update(sf::seconds(0.25f), [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == sf::seconds(0.75f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.update(sf::seconds(3.5f), [&]() -> bool
		{
			count++;
			return false;
		});

		CHECK(count == 4);
		CHECK(val.currentTime == sf::seconds(3.25f));
		CHECK(val.timeout == sf::seconds(1.f));

		val.update(sf::Time::Zero, [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 7);
		CHECK(val.currentTime == sf::seconds(0.25f));
		CHECK(val.timeout == sf::seconds(1.f));
	}
	SECTION("float and func returns bool")
	{
		ElapsedTimef val(1.f);

		auto count = 0;

		val.update(3.5f, [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == 0.5f);
		CHECK(val.timeout == 1.f);

		val.update(0.25f, [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 3);
		CHECK(val.currentTime == 0.75f);
		CHECK(val.timeout == 1.f);

		val.update(3.5f, [&]() -> bool
		{
			count++;
			return false;
		});

		CHECK(count == 4);
		CHECK(val.currentTime == 3.25f);
		CHECK(val.timeout == 1.f);

		val.update(0.f, [&]() -> bool
		{
			count++;
			return true;
		});

		CHECK(count == 7);
		CHECK(val.currentTime == 0.25f);
		CHECK(val.timeout == 1.f);
	}
}
