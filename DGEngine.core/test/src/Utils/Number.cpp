#include "catch2/catch_test_macros.hpp"

#include "Utils/Number.h"

TEST_CASE("Number32::constructor", "Utils")
{
	SECTION("default")
	{
		Number32 num;

		CHECK(num.isInt32() == true);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == 0);
		CHECK(num.getInt64() == 0);
		CHECK(num.getUInt32() == 0u);
		CHECK(num.getFloat() == 0.f);
		CHECK(num.getDouble() == 0.0);
	}
	SECTION("int")
	{
		Number32 num(10);

		CHECK(num.isInt32() == true);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == 10);
		CHECK(num.getInt64() == 10);
		CHECK(num.getUInt32() == 10u);
		CHECK(num.getFloat() == 10.f);
		CHECK(num.getDouble() == 10.0);
	}
	SECTION("negative int")
	{
		Number32 num(-10);

		CHECK(num.isInt32() == true);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == -10);
		CHECK(num.getInt64() == -10);
		CHECK(num.getUInt32() != 10u);
		CHECK(num.getFloat() == -10.f);
		CHECK(num.getDouble() == -10.0);
	}
	SECTION("unsigned")
	{
		Number32 num(10u);

		CHECK(num.isInt32() == false);
		CHECK(num.isUInt32() == true);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == 10);
		CHECK(num.getInt64() == 10);
		CHECK(num.getUInt32() == 10u);
		CHECK(num.getFloat() == 10.f);
		CHECK(num.getDouble() == 10.0);
	}
	SECTION("float")
	{
		Number32 num(10.5f);

		CHECK(num.isInt32() == false);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == true);
		CHECK(num.getInt32() == 10);
		CHECK(num.getInt64() == 10);
		CHECK(num.getUInt32() == 10u);
		CHECK(num.getFloat() == 10.5f);
		CHECK(num.getDouble() == 10.5);
	}
	SECTION("negative float")
	{
		Number32 num(-10.5f);

		CHECK(num.isInt32() == false);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == true);
		CHECK(num.getInt32() == -10);
		CHECK(num.getInt64() == -10);
		CHECK(num.getUInt32() != 10u);
		CHECK(num.getFloat() == -10.5f);
		CHECK(num.getDouble() == -10.5);
	}
}

TEST_CASE("Number32::set", "Utils")
{
	SECTION("int")
	{
		Number32 num(-20.5f);

		CHECK(num.isInt32() == false);

		num.setInt32(10);

		CHECK(num.isInt32() == true);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == 10);
		CHECK(num.getInt64() == 10);
		CHECK(num.getUInt32() == 10u);
		CHECK(num.getFloat() == 10.f);
		CHECK(num.getDouble() == 10.0);
	}
	SECTION("unsigned")
	{
		Number32 num(-20);

		CHECK(num.isUInt32() == false);

		num.setUInt32(10);

		CHECK(num.isInt32() == false);
		CHECK(num.isUInt32() == true);
		CHECK(num.isFloat() == false);
		CHECK(num.getInt32() == 10);
		CHECK(num.getInt64() == 10);
		CHECK(num.getUInt32() == 10u);
		CHECK(num.getFloat() == 10.f);
		CHECK(num.getDouble() == 10.0);
	}
	SECTION("float")
	{
		Number32 num(-20);

		CHECK(num.isFloat() == false);

		num.setFloat(-1.5);

		CHECK(num.isInt32() == false);
		CHECK(num.isUInt32() == false);
		CHECK(num.isFloat() == true);
		CHECK(num.getInt32() == -1);
		CHECK(num.getInt64() == -1);
		CHECK(num.getUInt32() != 1u);
		CHECK(num.getFloat() == -1.5f);
		CHECK(num.getDouble() == -1.5);
	}
}

TEST_CASE("Number32::+=", "Utils")
{
	SECTION("int32_t")
	{
		Number32 num(2);

		num += 2;

		CHECK(num.isInt32() == true);
		CHECK(num.getInt32() == 4);
	}
	SECTION("uint32_t")
	{
		Number32 num(2u);

		num += 2u;

		CHECK(num.isUInt32() == true);
		CHECK(num.getInt32() == 4);
	}
	SECTION("int64_t")
	{
		Number32 num(2);

		num += int64_t(2);

		CHECK(num.isInt32() == true);
		CHECK(num.getInt32() == 4);
	}
	SECTION("float")
	{
		Number32 num(2.f);

		num += 0.5f;

		CHECK(num.isFloat() == true);
		CHECK(num.getFloat() == 2.5f);
	}
	SECTION("double")
	{
		Number32 num(2.f);

		num += 0.5;

		CHECK(num.isFloat() == true);
		CHECK(num.getDouble() == 2.5);
	}
}

TEST_CASE("Number32::-=", "Utils")
{
	SECTION("int32_t")
	{
		Number32 num(2);

		num -= 4;

		CHECK(num.isInt32() == true);
		CHECK(num.getInt32() == -2);
	}
	SECTION("uint32_t")
	{
		Number32 num(2u);

		num -= 2u;

		CHECK(num.isUInt32() == true);
		CHECK(num.getInt32() == 0);

		num -= 2u;

		CHECK(num.isUInt32() == true);
		CHECK(num.getUInt32() > 1000000000);
		CHECK(num.getInt32() == -2);
	}
	SECTION("int64_t")
	{
		Number32 num(2);

		num -= int64_t(4);

		CHECK(num.isInt32() == true);
		CHECK(num.getInt32() == -2);
	}
	SECTION("float")
	{
		Number32 num(2.f);

		num -= 4.5f;

		CHECK(num.isFloat() == true);
		CHECK(num.getFloat() == -2.5f);
	}
	SECTION("double")
	{
		Number32 num(2.f);

		num -= 4.5;

		CHECK(num.isFloat() == true);
		CHECK(num.getDouble() == -2.5);
	}
}

TEST_CASE("Number32::==", "Utils")
{
	SECTION("equal")
	{
		Number32 num1(-2.f);
		Number32 num2(-2.f);

		CHECK(num1 == num2);
	}
	SECTION("not equal")
	{
		Number32 num1(2);
		Number32 num2(3);

		CHECK(num1 != num2);
	}
	SECTION("not equal")
	{
		Number32 num1(2);
		Number32 num2(2u);

		CHECK(num1 != num2);
		CHECK(num1.getInt32() == num2.getInt32());
		CHECK(num1.getUInt32() == num2.getUInt32());
		CHECK(num1.getFloat() == num2.getFloat());
	}
	SECTION("not equal")
	{
		Number32 num1(-2);
		Number32 num2(-2.f);

		CHECK(num1 != num2);
		CHECK(num1.getInt32() == num2.getInt32());
		CHECK(num1.getUInt32() == num2.getUInt32());
		CHECK(num1.getFloat() == num2.getFloat());
	}
	SECTION("equal int32_t")
	{
		Number32 num(-2);

		CHECK(num == -2);
	}
	SECTION("not equal int32_t")
	{
		Number32 num(2);

		CHECK(num != 3);
	}
	SECTION("not equal int32_t")
	{
		Number32 num(2u);

		CHECK(num != 2);
	}
	SECTION("not equal int32_t")
	{
		Number32 num(2.f);

		CHECK(num != 2);
	}
	SECTION("equal uint32_t")
	{
		Number32 num(2u);

		CHECK(num == 2u);
	}
	SECTION("not equal uint32_t")
	{
		Number32 num(2);

		CHECK(num != 3);
	}
	SECTION("not equal uint32_t")
	{
		Number32 num(2);

		CHECK(num != 2u);
	}
	SECTION("equal int64_t")
	{
		Number32 num(-2);

		CHECK(num == int64_t(-2));
	}
	SECTION("not equal int64_t")
	{
		Number32 num(2);

		CHECK(num != int64_t(3));
	}
	SECTION("not equal int64_t")
	{
		Number32 num(2.f);

		CHECK(num != int64_t(2));
	}
	SECTION("equal float")
	{
		Number32 num(-2.f);

		CHECK(num == -2.f);
	}
	SECTION("not equal float")
	{
		Number32 num(2.f);

		CHECK(num != 3.f);
	}
	SECTION("equal double")
	{
		Number32 num(-2.f);

		CHECK(num == -2.0);
	}
	SECTION("not equal double")
	{
		Number32 num(2.f);

		CHECK(num != 3.0);
	}
}
