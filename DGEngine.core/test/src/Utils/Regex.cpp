#include "catch2/catch_test_macros.hpp"

#include "Utils/Regex.h"

TEST_CASE("Regex::constructor", "Utils")
{
	SECTION("default")
	{
		Regex regex;

		CHECK(regex.isValid() == false);
		CHECK(regex.match("") == false);
		CHECK(regex.match("123") == false);
		CHECK(regex.match("test") == false);
	}
	SECTION("valid regex")
	{
		Regex regex("\\d+");

		CHECK(regex.isValid() == true);
		CHECK(regex.match("") == false);
		CHECK(regex.match("123") == true);
		CHECK(regex.match("test") == false);
	}
	SECTION("invalid regex")
	{
		Regex regex("[\\d+");

		CHECK(regex.isValid() == false);
		CHECK(regex.match("") == false);
		CHECK(regex.match("123") == false);
		CHECK(regex.match("test") == false);
	}
}

TEST_CASE("Regex::match", "Utils")
{
	SECTION("member match")
	{
		Regex regex("\\d+");

		CHECK(regex.isValid() == true);
		CHECK(regex.match("") == false);
		CHECK(regex.match("123") == true);
		CHECK(regex.match("test") == false);
	}
	SECTION("static match")
	{
		CHECK(Regex::match("\\d+", "") == false);
		CHECK(Regex::match("\\d+", "123") == true);
		CHECK(Regex::match("\\d+", "test") == false);
	}
	SECTION("invalid static match")
	{
		CHECK(Regex::match("", "") == false);
		CHECK(Regex::match("", "test") == false);
		CHECK(Regex::match("[\\d+", "") == false);
		CHECK(Regex::match("[\\d+", "123") == false);
	}
	SECTION("regexes")
	{
		CHECK(Regex::match("\\d+", "") == false);
		CHECK(Regex::match("\\d+", "123") == true);
		CHECK(Regex::match("\\d+", " 123 ") == false);
		CHECK(Regex::match("\\d+", "-123") == false);
		CHECK(Regex::match("-?\\d+", "123") == true);
		CHECK(Regex::match("-?\\d+", "-123") == true);
		CHECK(Regex::match("\\D+", "test") == true);
		CHECK(Regex::match("\\D+", "test1") == false);
		CHECK(Regex::match("\\s+\\d+\\s+", " 123\t") == true);
		CHECK(Regex::match("\\w+", "test") == true);
		CHECK(Regex::match("\\w+", "") == false);
		CHECK(Regex::match("\\w+", "abzdefghijklmnopqrstuvwxyz") == true);
		CHECK(Regex::match("[a-zA-Z]*", "Testing") == true);
		CHECK(Regex::match("[a-zA-Z]*", "") == false);
		CHECK(Regex::match("[^abc]+", "a test") == false);
		CHECK(Regex::match("[^abc]+", "1 test") == true);
		CHECK(Regex::match(".*esting", "\tTesting") == true);
		CHECK(Regex::match(".*esting", "esting") == true);
		CHECK(Regex::match(".+", "àêíöÙ") == true);
	}
}
