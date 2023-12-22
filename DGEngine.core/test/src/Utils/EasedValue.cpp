#include "catch2/catch_test_macros.hpp"

#include "Utils/EasedValue.h"

TEST_CASE("EasedValuef::constructor", "Utils")
{
	SECTION("default")
	{
		EasedValuef val;

		CHECK(val.get() == 0.f);
		CHECK(val.getFinal() == 0.f);
	}
	SECTION("float")
	{
		EasedValuef val(2.f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);
	}
	SECTION("different easing function")
	{
		EasedValue<float, EasingFunctions::easeLinear> val(2.f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);
	}
}

TEST_CASE("EasedValuef::get+set+update", "Utils")
{
	SECTION("stop time not set")
	{
		EasedValuef val(2.f);

		val.update(0.5);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);
	}
	SECTION("stop time set")
	{
		EasedValuef val;

		val.set(2.f, 10.f);

		CHECK(val.get() == 0.f);
		CHECK(val.getFinal() == 2.f);
	}
	SECTION("stop time set and updated")
	{
		EasedValuef val;

		val.set(2.f, 10.f);
		val.update(2.5f);

		CHECK(val.get() < 1.5f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() < 2.f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() < 2.f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);
	}
	SECTION("stop time set and updated with linear function")
	{
		EasedValue<float, EasingFunctions::easeLinear> val;

		val.set(2.f, 10.f);
		val.update(2.5f);

		CHECK(val.get() == 0.5f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 1.f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 1.5f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);

		val.update(2.5f);

		CHECK(val.get() == 2.f);
		CHECK(val.getFinal() == 2.f);
	}
}
