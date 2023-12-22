#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "Utils/PairXY.h"
#include "Utils/Utils.h"

TEST_CASE("Utils::normalizeNumber", "Utils")
{
	SECTION("no range")
	{
		PairInt32 inputRange(0, 0);
		PairInt32 outputRange(100, 100);

		CHECK(Utils::normalizeNumber(-1, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(0, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(100, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(200, inputRange, outputRange) == 100);

		outputRange.y = 200;

		CHECK(Utils::normalizeNumber(-1, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(0, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(100, inputRange, outputRange) == 100);
		CHECK(Utils::normalizeNumber(200, inputRange, outputRange) == 100);
	}
	SECTION("positive range")
	{
		PairInt32 inputRange(0, 100);
		PairInt32 outputRange(1000, 2000);

		CHECK(Utils::normalizeNumber(-1, inputRange, outputRange) == 1000);
		CHECK(Utils::normalizeNumber(0, inputRange, outputRange) == 1000);
		CHECK(Utils::normalizeNumber(25, inputRange, outputRange) == 1250);
		CHECK(Utils::normalizeNumber(50, inputRange, outputRange) == 1500);
		CHECK(Utils::normalizeNumber(75, inputRange, outputRange) == 1750);
		CHECK(Utils::normalizeNumber(100, inputRange, outputRange) == 2000);
		CHECK(Utils::normalizeNumber(200, inputRange, outputRange) == 2000);
	}
	SECTION("negative input range")
	{
		PairInt32 inputRange(-100, -50);
		PairInt32 outputRange(1000, 2000);

		CHECK(Utils::normalizeNumber(-200, inputRange, outputRange) == 1000);
		CHECK(Utils::normalizeNumber(-100, inputRange, outputRange) == 1000);
		CHECK(Utils::normalizeNumber(-75, inputRange, outputRange) == 1500);
		CHECK(Utils::normalizeNumber(-50, inputRange, outputRange) == 2000);
		CHECK(Utils::normalizeNumber(-10, inputRange, outputRange) == 2000);
	}
	SECTION("negative output range")
	{
		PairInt32 inputRange(0, 100);
		PairInt32 outputRange(-2000, -1000);

		CHECK(Utils::normalizeNumber(-1, inputRange, outputRange) == -2000);
		CHECK(Utils::normalizeNumber(0, inputRange, outputRange) == -2000);
		CHECK(Utils::normalizeNumber(50, inputRange, outputRange) == -1500);
		CHECK(Utils::normalizeNumber(100, inputRange, outputRange) == -1000);
		CHECK(Utils::normalizeNumber(200, inputRange, outputRange) == -1000);
	}
	SECTION("negative input and output range")
	{
		PairInt32 inputRange(-100, -50);
		PairInt32 outputRange(-2000, -1000);

		CHECK(Utils::normalizeNumber(-200, inputRange, outputRange) == -2000);
		CHECK(Utils::normalizeNumber(-100, inputRange, outputRange) == -2000);
		CHECK(Utils::normalizeNumber(-75, inputRange, outputRange) == -1500);
		CHECK(Utils::normalizeNumber(-50, inputRange, outputRange) == -1000);
		CHECK(Utils::normalizeNumber(-10, inputRange, outputRange) == -1000);
	}
	SECTION("float range")
	{
		PairFloat inputRange(0.f, 100.f);
		PairFloat outputRange(10.5f, 20.5f);

		CHECK(Utils::normalizeNumber(-1, inputRange, outputRange) == 10.f);
		CHECK(Utils::normalizeNumber(0, inputRange, outputRange) == 10.f);
		CHECK(Utils::normalizeNumber(50, inputRange, outputRange) == 15.f);
		CHECK(Utils::normalizeNumber(100, inputRange, outputRange) == 20.f);
		CHECK(Utils::normalizeNumber(200, inputRange, outputRange) == 20.f);
	}
}

TEST_CASE("Utils::removeEmptyLines", "Utils")
{
	CHECK(Utils::removeEmptyLines("") == "");
	CHECK(Utils::removeEmptyLines("test") == "test");
	CHECK(Utils::removeEmptyLines("test\ntest") == "test\ntest");
	CHECK(Utils::removeEmptyLines("test\n\ntest") == "test\ntest");
	CHECK(Utils::removeEmptyLines("\ntest\ntest\n") == "test\ntest");
	CHECK(Utils::removeEmptyLines("\ntest\n\ntest\n") == "test\ntest");
	CHECK(Utils::removeEmptyLines("\n\ntest\n\ntest\n\n") == "test\ntest");
	CHECK(Utils::removeEmptyLines("\r\n") == "\r");
	CHECK(Utils::removeEmptyLines("\n\n\n") == "");
}

TEST_CASE("Utils::replaceStringInPlace", "Utils")
{
	std::string str("this is a test string");
	auto strOriginal = str;

	SECTION("")
	{
		Utils::replaceStringInPlace(str, "test ", "");
		CHECK(str == "this is a string");
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "test", "new");
		CHECK(str == "this is a new string");
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, " ", "_");
		CHECK(str == "this_is_a_test_string");
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "is ", "");
		CHECK(str == "tha test string");
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "", "");
		CHECK(str == strOriginal);
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "", "new");
		CHECK(str == strOriginal);
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "  ", "");
		CHECK(str == strOriginal);
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "IS", "");
		CHECK(str == strOriginal);
	}
	SECTION("")
	{
		Utils::replaceStringInPlace(str, "notFound", "found");
		CHECK(str == strOriginal);
	}
	SECTION("")
	{
		std::string str2("");
		Utils::replaceStringInPlace(str2, "notFound", "found");
		CHECK(str2.empty());
	}
}

TEST_CASE("Utils::round", "Utils")
{
	SECTION("float")
	{
		CHECK(Utils::round(0.f, 0) == 0.f);
		CHECK(Utils::round(0.f, 2) == 0.f);
		CHECK(Utils::round(-1.f, 0) == -1.f);
		CHECK(Utils::round(-1.f, 2) == -1.f);
		CHECK(Utils::round(123.f, 0) == 123.f);
		CHECK(Utils::round(123.f, 2) == 123.f);
		CHECK(Utils::round(-123.f, 0) == -123.f);
		CHECK(Utils::round(-123.f, 2) == -123.f);
		CHECK(Utils::round(123.4447f, 2) == 123.44f);
		CHECK(Utils::round(-123.4447f, 2) == -123.44f);
		CHECK(Utils::round(123.4467f, 2) == 123.45f);
		CHECK(Utils::round(-123.4467f, 2) == -123.45f);
	}
	SECTION("double")
	{
		CHECK(Utils::round(-123.4464321, 4) == -123.4464);
		CHECK(Utils::round(-9876543.4464321, 0) == -9876543.0);
		CHECK(Utils::round(-9876543.4464321, 1) == -9876543.4);
		CHECK(Utils::round(-9876543.4464321, 4) == -9876543.4464);
	}
}

TEST_CASE("Utils::splitString", "Utils")
{
	auto data = GENERATE(table<std::string, std::string, std::vector<std::string_view>>
	({
		{"",               ".", {}},
		{".",              ".", {}},
		{"..",             ".", {}},
		{" .. ",           ".", {" ", " "}},
		{"  ",             ".", {"  "}},
		{"test..",         ".", {"test"}},
		{"test.string",    ".", {"test", "string"}},
		{"test.string.",   ".", {"test", "string"}},
		{"test..string",   ".", {"test", "string"}},
		{"test.string.\n", ".", {"test", "string", "\n"}},
		{"\n.test.string", ".", {"\n", "test", "string"}},
		{"test. ",         ".", {"test", " "}},
		{" .test",         ".", {" ", "test"}},
		{"test.string",    " ", {"test.string"}},
		{"\n \n \n",       " ", {"\n", "\n", "\n"}}
	}));

	CHECK(Utils::splitString(std::get<0>(data), std::get<1>(data)) == std::get<2>(data));
}

TEST_CASE("Utils::splitStringIn2", "Utils")
{
	SECTION("std::string")
	{
		auto data = GENERATE(table<std::string, char, std::pair<std::string, std::string>>
		({
			{"",             '.', {"", ""}},
			{".",            '.', {"", ""}},
			{"..",           '.', {"", "."}},
			{"...",          '.', {"", ".."}},
			{"  ",           '.', {"  ", ""}},
			{"test.string",  '.', {"test", "string"}},
			{"test.",        '.', {"test", ""}},
			{"test..",       '.', {"test", "."}},
			{"test..string", '.', {"test", ".string"}},
			{"test. ",       '.', {"test", " "}},
			{"test",         '.', {"test", ""}},
			{"..test",       '.', {"", ".test"}},
			{"test.string",  ' ', {"test.string", ""}}
		}));

		CHECK(Utils::splitStringIn2(std::get<0>(data), std::get<1>(data)) == std::get<2>(data));
	}
	SECTION("std::string_view")
	{
		auto data = GENERATE(table<std::string_view, char, std::pair<std::string_view, std::string_view>>
		({
			{"",             '.', {"", ""}},
			{".",            '.', {"", ""}},
			{"..",           '.', {"", "."}},
			{"...",          '.', {"", ".."}},
			{"  ",           '.', {"  ", ""}},
			{"test.string",  '.', {"test", "string"}},
			{"test.",        '.', {"test", ""}},
			{"test..",       '.', {"test", "."}},
			{"test..string", '.', {"test", ".string"}},
			{"test. ",       '.', {"test", " "}},
			{"test",         '.', {"test", ""}},
			{"..test",       '.', {"", ".test"}},
			{"test.string",  ' ', {"test.string", ""}}
		}));

		CHECK(Utils::splitStringIn2(std::get<0>(data), std::get<1>(data)) == std::get<2>(data));
	}
}

TEST_CASE("Utils::strToNumber", "Utils")
{
	CHECK(Utils::strToNumber<int8_t>("0") == 0);
	CHECK(Utils::strToNumber<int8_t>("127") == 127);
	CHECK(Utils::strToNumber<int8_t>("-127") == -127);
	CHECK(Utils::strToNumber<int8_t>("256") == 0);
	CHECK(Utils::strToNumber<uint8_t>("1") == 1);
	CHECK(Utils::strToNumber<uint8_t>("255") == 255);
	CHECK(Utils::strToNumber<uint8_t>("256") == 0);
	CHECK(Utils::strToNumber<int32_t>("98765432") == 98765432);
	CHECK(Utils::strToNumber<int32_t>("-98765432") == -98765432);
	CHECK(Utils::strToNumber<int32_t>("9876543210") == 0);
	CHECK(Utils::strToNumber<uint32_t>("98765432") == 98765432);
	CHECK(Utils::strToNumber<uint32_t>("9876543210") == 0);
	CHECK(Utils::strToNumber<int64_t>("9876543210") == 9876543210);
	CHECK(Utils::strToNumber<int64_t>("-9876543210") == -9876543210);
	CHECK(Utils::strToNumber<uint64_t>("98765432109876") == 98765432109876);
	CHECK(Utils::strToNumber<uint64_t>("98765432109876000000") == 0);
	CHECK(Utils::strToNumber<float>("0.0") == 0.f);
	CHECK(Utils::strToNumber<float>(".1") == 0.1f);
	CHECK(Utils::strToNumber<float>("1234.5678") == 1234.5678f);
	CHECK(Utils::strToNumber<float>("-1234.5678") == -1234.5678f);
	CHECK(Utils::strToNumber<double>("9876543210.123") == 9876543210.123);
	CHECK(Utils::strToNumber<double>("-9876543210.123") == -9876543210.123);
}

TEST_CASE("Utils::strToNumberOpt", "Utils")
{
	CHECK(Utils::strToNumberOpt<int8_t>("0").has_value() == true);
	CHECK(Utils::strToNumberOpt<int8_t>("256").has_value() == false);
	CHECK(Utils::strToNumberOpt<uint8_t>("1").has_value() == true);
	CHECK(Utils::strToNumberOpt<uint8_t>("256").has_value() == false);
	CHECK(Utils::strToNumberOpt<int32_t>("98765432").has_value() == true);
	CHECK(Utils::strToNumberOpt<int32_t>("9876543210").has_value() == false);
	CHECK(Utils::strToNumberOpt<uint32_t>("98765432").has_value() == true);
	CHECK(Utils::strToNumberOpt<uint32_t>("9876543210").has_value() == false);
	CHECK(Utils::strToNumberOpt<int64_t>("9876543210").has_value() == true);
	CHECK(Utils::strToNumberOpt<int64_t>("98765432109876000000").has_value() == false);
	CHECK(Utils::strToNumberOpt<uint64_t>("98765432109876").has_value() == true);
	CHECK(Utils::strToNumberOpt<uint64_t>("98765432109876000000").has_value() == false);
}

TEST_CASE("Utils::toLower", "Utils")
{
	CHECK(Utils::toLower("") == "");
	CHECK(Utils::toLower(" ") == " ");
	CHECK(Utils::toLower(".") == ".");
	CHECK(Utils::toLower("test") == "test");
	CHECK(Utils::toLower("TEST") == "test");
	CHECK(Utils::toLower(" TEST ") == " test ");
	CHECK(Utils::toLower("áèîöu") == "áèîöu");
	CHECK(Utils::toLower("ÁÈÎÖU") == "ÁÈÎÖu");
}

TEST_CASE("Utils::toString", "Utils")
{
	SECTION("double")
	{
		CHECK(Utils::toString(0.0) == "0.0");
		CHECK(Utils::toString(-1.0) == "-1.0");
		CHECK(Utils::toString(987654.0) == "987654.0");
		CHECK(Utils::toString(-123456789.87654) == "-123456789.87654");
	}
	SECTION("int")
	{
		CHECK(Utils::toString(0) == "0");
		CHECK(Utils::toString(-1) == "-1");
		CHECK(Utils::toString(987654) == "987654");
		CHECK(Utils::toString(-123456789) == "-123456789");
	}
	SECTION("long")
	{
		CHECK(Utils::toString((long)0) == "0");
		CHECK(Utils::toString((long)-1) == "-1");
		CHECK(Utils::toString((long)987654) == "987654");
		CHECK(Utils::toString((long)-123456789) == "-123456789");
	}
	SECTION("long long")
	{
		CHECK(Utils::toString((long long)0) == "0");
		CHECK(Utils::toString((long long)-1) == "-1");
		CHECK(Utils::toString((long long)987654) == "987654");
		CHECK(Utils::toString((long long)-123456789) == "-123456789");
	}
	SECTION("unsigned int")
	{
		CHECK(Utils::toString((unsigned int)0) == "0");
		CHECK(Utils::toString((unsigned int)1) == "1");
		CHECK(Utils::toString((unsigned int)987654) == "987654");
		CHECK(Utils::toString((unsigned int)123456789) == "123456789");
	}
	SECTION("unsigned long")
	{
		CHECK(Utils::toString((unsigned long)0) == "0");
		CHECK(Utils::toString((unsigned long)1) == "1");
		CHECK(Utils::toString((unsigned long)987654) == "987654");
		CHECK(Utils::toString((unsigned long)123456789) == "123456789");
	}
	SECTION("unsigned long long")
	{
		CHECK(Utils::toString((unsigned long long)0) == "0");
		CHECK(Utils::toString((unsigned long long)1) == "1");
		CHECK(Utils::toString((unsigned long long)987654) == "987654");
		CHECK(Utils::toString((unsigned long long)123456789) == "123456789");
	}
}

TEST_CASE("Utils::toUpper", "Utils")
{
	CHECK(Utils::toUpper("") == "");
	CHECK(Utils::toUpper(" ") == " ");
	CHECK(Utils::toUpper(".") == ".");
	CHECK(Utils::toUpper("test") == "TEST");
	CHECK(Utils::toUpper("TEST") == "TEST");
	CHECK(Utils::toUpper(" TEST ") == " TEST ");
	CHECK(Utils::toUpper("áèîöu") == "áèîöU");
	CHECK(Utils::toUpper("ÁÈÎÖU") == "ÁÈÎÖU");
}

TEST_CASE("Utils::trim", "Utils")
{
	CHECK(Utils::trim("") == "");
	CHECK(Utils::trim("", "") == "");
	CHECK(Utils::trim(" ") == "");
	CHECK(Utils::trim(" ", "") == " ");
	CHECK(Utils::trim("  ") == "");
	CHECK(Utils::trim("\t") == "");
	CHECK(Utils::trim(" \t") == "");
	CHECK(Utils::trim("\t \t\t\t  ") == "");
	CHECK(Utils::trim("\t \t\t\t  ", "\t") == " \t\t\t  ");
	CHECK(Utils::trim("  \t \t\t\t", "\t") == "  \t ");
	CHECK(Utils::trim("test string") == "test string");
	CHECK(Utils::trim(" test string") == "test string");
	CHECK(Utils::trim("test string ") == "test string");
	CHECK(Utils::trim(" test string ") == "test string");
	CHECK(Utils::trim("  test string  ") == "test string");
	CHECK(Utils::trim("\ttest string") == "test string");
	CHECK(Utils::trim("test string\t") == "test string");
	CHECK(Utils::trim("\ttest string\t") == "test string");
	CHECK(Utils::trim("\t\ttest string\t\t") == "test string");
	CHECK(Utils::trim("__ test string __", "_") == " test string ");
	CHECK(Utils::trim("__ test string __", "__") == " test string ");
	CHECK(Utils::trim("__ test string __", "___") == " test string ");
}

TEST_CASE("Utils::trimEnd", "Utils")
{
	CHECK(Utils::trimEnd("") == "");
	CHECK(Utils::trimEnd("", "") == "");
	CHECK(Utils::trimEnd(" ") == "");
	CHECK(Utils::trimEnd(" ", "") == " ");
	CHECK(Utils::trimEnd("  ") == "");
	CHECK(Utils::trimEnd("\t") == "");
	CHECK(Utils::trimEnd(" \t") == "");
	CHECK(Utils::trimEnd("\t \t\t\t  ") == "");
	CHECK(Utils::trimEnd("\t \t\t\t  ", "\t") == "\t \t\t\t  ");
	CHECK(Utils::trimEnd("  \t \t\t\t", "\t") == "  \t ");
	CHECK(Utils::trimEnd("test string") == "test string");
	CHECK(Utils::trimEnd(" test string") == " test string");
	CHECK(Utils::trimEnd("test string ") == "test string");
	CHECK(Utils::trimEnd(" test string ") == " test string");
	CHECK(Utils::trimEnd("  test string  ") == "  test string");
	CHECK(Utils::trimEnd("\ttest string") == "\ttest string");
	CHECK(Utils::trimEnd("test string\t") == "test string");
	CHECK(Utils::trimEnd("\ttest string\t") == "\ttest string");
	CHECK(Utils::trimEnd("\t\ttest string\t\t") == "\t\ttest string");
	CHECK(Utils::trimEnd("__ test string __", "_") == "__ test string ");
	CHECK(Utils::trimEnd("__ test string __", "__") == "__ test string ");
	CHECK(Utils::trimEnd("__ test string __", "___") == "__ test string ");
}

TEST_CASE("Utils::trimStart", "Utils")
{
	CHECK(Utils::trimStart("") == "");
	CHECK(Utils::trimStart("", "") == "");
	CHECK(Utils::trimStart(" ") == "");
	CHECK(Utils::trimStart(" ", "") == " ");
	CHECK(Utils::trimStart("  ") == "");
	CHECK(Utils::trimStart("\t") == "");
	CHECK(Utils::trimStart(" \t") == "");
	CHECK(Utils::trimStart("\t \t\t\t  ") == "");
	CHECK(Utils::trimStart("\t \t\t\t  ", "\t") == " \t\t\t  ");
	CHECK(Utils::trimStart("  \t \t\t\t", "\t") == "  \t \t\t\t");
	CHECK(Utils::trimStart("test string") == "test string");
	CHECK(Utils::trimStart(" test string") == "test string");
	CHECK(Utils::trimStart("test string ") == "test string ");
	CHECK(Utils::trimStart(" test string ") == "test string ");
	CHECK(Utils::trimStart("  test string  ") == "test string  ");
	CHECK(Utils::trimStart("\ttest string") == "test string");
	CHECK(Utils::trimStart("test string\t") == "test string\t");
	CHECK(Utils::trimStart("\ttest string\t") == "test string\t");
	CHECK(Utils::trimStart("\t\ttest string\t\t") == "test string\t\t");
	CHECK(Utils::trimStart("__ test string __", "_") == " test string __");
	CHECK(Utils::trimStart("__ test string __", "__") == " test string __");
	CHECK(Utils::trimStart("__ test string __", "___") == " test string __");
}

TEST_CASE("Utils::wordWrap", "Utils")
{
	std::string str("This is a very long string to test the word wrap algorithm.");

	CHECK(Utils::wordWrap(str, -1) == str);
	CHECK(Utils::wordWrap(str, 0) == "This\nis\na\nvery\nlong\nstring\nto\ntest\nthe\nword\nwrap\nalgorithm.");
	CHECK(Utils::wordWrap(str, 1) == "This\nis\na\nvery\nlong\nstring\nto\ntest\nthe\nword\nwrap\nalgorithm.");
	CHECK(Utils::wordWrap(str, 20) == "This is a very long\nstring to test the\nword wrap\nalgorithm.");
	CHECK(Utils::wordWrap(str, (int)str.size() - 1) == "This is a very long string to test the word wrap\nalgorithm.");
	CHECK(Utils::wordWrap(str, (int)str.size()) == str);
	CHECK(Utils::wordWrap(str, (int)str.size() + 1) == str);
	CHECK(Utils::wordWrap(str, (int)str.size() + 10) == str);
	CHECK(Utils::wordWrap("test string", 4) == "test\nstring");
	CHECK(Utils::wordWrap(" test string", 4) == " test\nstring");
	CHECK(Utils::wordWrap("\ttest string", 4) == "\ttest\nstring");
	CHECK(Utils::wordWrap("test", -1) == "test");
	CHECK(Utils::wordWrap("t e s t", -1) == "t e s t");
	CHECK(Utils::wordWrap("t e s t", 0) == "t\ne\ns\nt");
	CHECK(Utils::wordWrap("t e s t", 1) == "t\ne\ns\nt");
	CHECK(Utils::wordWrap("t e s t", 2) == "t\ne\ns\nt");
	CHECK(Utils::wordWrap("t e s t ", 2) == "t\ne\ns\nt ");
	CHECK(Utils::wordWrap("t e s t  ", 2) == "t\ne\ns\nt  ");
	CHECK(Utils::wordWrap("t e s t   ", 2) == "t\ne\ns\nt   ");
	CHECK(Utils::wordWrap("t e s t    ", 2) == "t\ne\ns\nt    ");
	CHECK(Utils::wordWrap(" t e s t", 2) == " t\ne\ns\nt");
	CHECK(Utils::wordWrap("  t e s t", 2) == " \nt\ne\ns\nt");
	CHECK(Utils::wordWrap("   t e s t", 2) == " \n t\ne\ns\nt");
	CHECK(Utils::wordWrap("    t e s t", 2) == " \n \nt\ne\ns\nt");
	CHECK(Utils::wordWrap("t e s t", 3) == "t e\ns t");
}
