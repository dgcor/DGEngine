#include "catch2/catch_test_macros.hpp"

#include "Game/Formula.h"
#include <unordered_map>
#include "Utils/Utils.h"

TEST_CASE("Formula::eval", "Formula")
{
	SECTION("constructor")
	{
		Formula formula;

		CHECK(formula.empty() == true);
		CHECK(formula.eval() == 0.0);
	}
	SECTION("constructor invalid")
	{
		Formula formula("text");

		CHECK(formula.empty() == false);
		CHECK(formula.eval() == 0.0);
		CHECK(formula.toString() == "text");
	}
	SECTION("constructor valid")
	{
		Formula formula("2.00 + 2 ");

		CHECK(formula.empty() == false);
		CHECK(formula.eval() == 4.0);
		CHECK(formula.toString() == "2.0 + 2.0");
	}
}

TEST_CASE("Formula::evalMaxString", "Formula")
{
	CHECK(Formula::evalMaxString(":rnd 0") == 0.0);
	CHECK(Formula::evalMaxString(":rnd 10") == 9.0);
	CHECK(Formula::evalMaxString(":rndf 0") == 0.0);
	CHECK(Formula::evalMaxString(":rndf 10") == 9.0);
	CHECK(Formula::evalMaxString(":rndn 0") == 0.0);
	CHECK(Formula::evalMaxString(":rndn 10") == 10.0);
}

TEST_CASE("Formula::evalMinString", "Formula")
{
	CHECK(Formula::evalMinString(":rnd 0") == 0.0);
	CHECK(Formula::evalMinString(":rnd 10") == 0.0);
	CHECK(Formula::evalMinString(":rndf 0") == 0.0);
	CHECK(Formula::evalMinString(":rndf 10") == 0.0);
	CHECK(Formula::evalMinString(":rndn 0") == 0.0);
	CHECK(Formula::evalMinString(":rndn 10") == 0.0);
}

TEST_CASE("Formula::evalString", "Formula")
{
	SECTION("basic math")
	{
		// +, -, *, /, %, ^
		CHECK(Formula::evalString("2 + 2") == 4.0);
		CHECK(Formula::evalString("2 + -3") == -1.0);
		CHECK(Formula::evalString("-2.5 * 10") == -25.0);
		CHECK(Formula::evalString("-2.5 / 10") == -0.25);
		CHECK(Formula::evalString("2 / 0") == 2);
		CHECK(Formula::evalString("-2.5 / 0") == -2.5);
		CHECK(Formula::evalString("3 % 2") == 1.0);
		CHECK(Formula::evalString("3 :mod 2") == 1.0);
		CHECK(Formula::evalString("-2.5 % 2") == -0.5);
		CHECK(Formula::evalString("10 ^ 3") == 1000.0);
		CHECK(Formula::evalString("10 :pow 3") == 1000.0);
		CHECK(Formula::evalString("10^6 + 5.5") == 1000005.5);
	}
	SECTION("parenthesis")
	{
		// parenthesis force order of operations
		CHECK(Formula::evalString("2 + 2 * 4 / 5") == 3.2);
		CHECK(Formula::evalString("2 + ((2 * 4) / 5)") == 3.6);
		CHECK(Formula::evalString("2 + ((((((2 * 4) / 5)") == 3.6);
		CHECK(Formula::evalString("2 + ((2 * 4) / 5)))))") == 3.6);
	}
	SECTION("operator override")
	{
		// operators override the previous one
		CHECK(Formula::evalString("2 * -4") == -2);
		CHECK(Formula::evalString("2 * -4") == -2);
		CHECK(Formula::evalString("2 :max -4") == -2);
	}
	SECTION("no op")
	{
		// no op = +
		CHECK(Formula::evalString("2 3") == 5.0);
	}
	SECTION("min max operations")
	{
		// :min, :max, :nvl, :neg, :negz, :pos, :posz
		CHECK(Formula::evalString("2 :min 5") == 2.0);
		CHECK(Formula::evalString("(-6) :min (-2)") == -6.0);

		CHECK(Formula::evalString("2 :max 5") == 5.0);
		CHECK(Formula::evalString("(-6) :max (-2)") == -2.0);

		CHECK(Formula::evalString("2 :nvl 5") == 2.0);
		CHECK(Formula::evalString("0 :nvl 5") == 5.0);
		CHECK(Formula::evalString("2 :zero 5") == 2.0);
		CHECK(Formula::evalString("0 :zero 5") == 5.0);

		CHECK(Formula::evalString("2 :neg 5") == 2.0);
		CHECK(Formula::evalString("0 :neg 5") == 0.0);
		CHECK(Formula::evalString("-2 :neg 5") == 5.0);

		CHECK(Formula::evalString("2 :negz 5") == 2.0);
		CHECK(Formula::evalString("0 :negz 5") == 5.0);
		CHECK(Formula::evalString("-2 :negz 5") == 5.0);

		CHECK(Formula::evalString("2 :pos 5") == 5.0);
		CHECK(Formula::evalString("0 :pos 5") == 0.0);
		CHECK(Formula::evalString("-2 :pos 5") == -2.0);

		CHECK(Formula::evalString("2 :posz 5") == 5.0);
		CHECK(Formula::evalString("0 :posz 5") == 5.0);
		CHECK(Formula::evalString("-2 :posz 5") == -2.0);
	}
	SECTION("random int")
	{
		Formula formula(":rnd 4");

		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 100; i++)
		{
			++hist[(int)formula.eval()];
		}

		CHECK(hist.contains(-1) == false);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(4) == true);
		CHECK(hist.contains(5) == false);

		CHECK(hist[0] > 2);
		CHECK(hist[1] > 2);
		CHECK(hist[2] > 2);
		CHECK(hist[3] > 2);
		CHECK(hist[4] > 2);

		CHECK(Formula::evalString(":rnd(5)") >= 0.0);
		CHECK(Formula::evalString(":rnd(0)") == 0.0);
		CHECK(Formula::evalString(":rnd(-5)") == 0.0);
	}
	SECTION("random float")
	{
		Formula formula(":rndf 4");

		for (auto i = 0; i < 10; i++)
		{
			auto val = formula.eval();
			CHECK(val >= 0.0);
			CHECK(val < 4.0);
		}

		CHECK(Formula::evalString(":rndf(5)") >= 0.0);
		CHECK(Formula::evalString(":rndf(0)") == 0.0);
		CHECK(Formula::evalString(":rndf(-5)") == 0.0);
	}
	SECTION("random normal distribution")
	{
		Formula formula(":rndn 4");

		std::unordered_map<int, int> hist;
		for (auto i = 0; i < 1000; i++)
		{
			++hist[(int)std::round(formula.eval())];
		}

		CHECK(hist.size() == 5);

		CHECK(hist.contains(-1) == false);
		CHECK(hist.contains(0) == true);
		CHECK(hist.contains(1) == true);
		CHECK(hist.contains(2) == true);
		CHECK(hist.contains(3) == true);
		CHECK(hist.contains(4) == true);
		CHECK(hist.contains(5) == false);

		CHECK(hist[0] < hist[1]);
		CHECK(hist[1] < hist[2]);
		CHECK(hist[2] > hist[3]);
		CHECK(hist[3] > hist[4]);

		CHECK(Formula::evalString(":rndn(5)") >= 0.0);
		CHECK(Formula::evalString(":rndn(0)") == 0.0);
		CHECK(Formula::evalString(":rndn(-5)") == 0.0);
	}
	SECTION("random static")
	{
		CHECK(Formula::evalString(":rnd(5)", -1) == 4.0);
		CHECK(Formula::evalString(":rnd(5)", -2) == 0.0);
		CHECK(Formula::evalString(":rnd(5)", 10) == 10.0);
		CHECK(Formula::evalString(":rnd(0)", 20) == 20.0);
		CHECK(Formula::evalString(":rnd(-5)", 30) == 30.0);

		CHECK(Formula::evalString(":rndf(5)", -1) == 4.0);
		CHECK(Formula::evalString(":rndf(5)", -2) == 0.0);
		CHECK(Formula::evalString(":rndf(5)", 10) == 10.0);
		CHECK(Formula::evalString(":rndf(0)", 20) == 20.0);
		CHECK(Formula::evalString(":rndf(-5)", 30) == 30.0);

		CHECK(Formula::evalString(":rndn(5)", -1) == 5.0);
		CHECK(Formula::evalString(":rndn(5)", -2) == 0.0);
		CHECK(Formula::evalString(":rndn(5)", 10) == 10.0);
		CHECK(Formula::evalString(":rndn(0)", 20) == 20.0);
		CHECK(Formula::evalString(":rndn(-5)", 30) == 30.0);

		CHECK(Formula(":rnd(5)").eval("0") >= 0.0);
		CHECK(Formula(":rnd(5)").eval("20") == 20.0);
		CHECK(Formula(":rnd(5)").eval("-3") == 0.0);
		CHECK(Formula(":rnd(5)").eval("min") == 0.0);
		CHECK(Formula(":rnd(5)").eval("max") == 4.0);
	}
	SECTION("round operations")
	{
		// :abs, :ceil, :floor, :trunc, :round
		CHECK(Formula::evalString(":abs 5") == 5.0);
		CHECK(Formula::evalString(":abs (-2)") == 2.0);

		CHECK(Formula::evalString(":ceil 5.2") == 6.0);
		CHECK(Formula::evalString(":ceil 5.6") == 6.0);
		CHECK(Formula::evalString(":ceil (-2.2)") == -2.0);
		CHECK(Formula::evalString(":ceil (-2.6)") == -2.0);

		CHECK(Formula::evalString(":floor 5.2") == 5.0);
		CHECK(Formula::evalString(":floor 5.6") == 5.0);
		CHECK(Formula::evalString(":floor (-2.2)") == -3.0);
		CHECK(Formula::evalString(":floor (-2.6)") == -3.0);

		CHECK(Formula::evalString(":trunc 5.2") == 5.0);
		CHECK(Formula::evalString(":trunc 5.6") == 5.0);
		CHECK(Formula::evalString(":trunc (-2.2)") == -2.0);
		CHECK(Formula::evalString(":trunc (-2.6)") == -2.0);

		CHECK(Formula::evalString(":round 5.2") == 5.0);
		CHECK(Formula::evalString(":round 5.6") == 6.0);
		CHECK(Formula::evalString(":round (-2.2)") == -2.0);
		CHECK(Formula::evalString(":round (-2.6)") == -3.0);
	}
	SECTION("math operations")
	{
		// :log, :ln, :sqrt, :cos, :sin, :tan
		CHECK(Formula::evalString(":log 1") == 0.0);
		CHECK(Utils::round(Formula::evalString(":log 2"), 2) == 0.30);
		CHECK(Utils::round(Formula::evalString(":log 3"), 2) == 0.48);
		CHECK(std::isinf(Formula::evalString(":log 0")) == true);
		CHECK(std::isnan(Formula::evalString(":log (-1)")) == true);

		CHECK(Formula::evalString(":ln 1") == 0.0);
		CHECK(Utils::round(Formula::evalString(":ln 2"), 2) == 0.69);
		CHECK(Utils::round(Formula::evalString(":ln 3"), 2) == 1.1);
		CHECK(std::isinf(Formula::evalString(":ln 0")) == true);
		CHECK(std::isnan(Formula::evalString(":ln (-1)")) == true);

		CHECK(Formula::evalString(":cos 0") == 1.0);
		CHECK(Utils::round(Formula::evalString(":cos 1"), 2) == 0.54);
		CHECK(Utils::round(Formula::evalString(":cos (-1)"), 2) == 0.54);
		CHECK(Utils::round(Formula::evalString(":cos (-100)"), 2) == 0.86);
		CHECK(Utils::round(Formula::evalString(":cos (3.1415 / 3)"), 2) == 0.5);

		CHECK(Formula::evalString(":sin 0") == 0.0);
		CHECK(Utils::round(Formula::evalString(":sin 1"), 2) == 0.84);
		CHECK(Utils::round(Formula::evalString(":sin (-1)"), 2) == -0.84);
		CHECK(Utils::round(Formula::evalString(":sin (-100)"), 2) == 0.51);
		CHECK(Utils::round(Formula::evalString(":sin (3.1415 / 6)"), 2) == 0.5);

		CHECK(Formula::evalString(":tan 0") == 0.0);
		CHECK(Utils::round(Formula::evalString(":tan 1"), 2) == 1.56);
		CHECK(Utils::round(Formula::evalString(":tan (-1)"), 2) == -1.56);
		CHECK(Utils::round(Formula::evalString(":tan (-100)"), 2) == 0.59);
		CHECK(Utils::round(Formula::evalString(":tan (3.1415 / 4)"), 2) == 1.0);
	}
}

TEST_CASE("Formula::toString", "Formula")
{
	CHECK(Formula("-2").toString() == "- 2.0");
	CHECK(Formula("2+2").toString() == "2.0 + 2.0");
	CHECK(Formula("2 * -4").toString() == "2.0 * - 4.0");
	CHECK(Formula("2 :max -4").toString() == "2.0 :max - 4.0");
	CHECK(Formula("2+((2*4)/5").toString() == "2.0 + ((2.0 * 4.0) / 5.0)");
	CHECK(Formula("2+((2*4)/5)").toString() == "2.0 + ((2.0 * 4.0) / 5.0)");
	CHECK(Formula("2+((2*4)/5))").toString() == "2.0 + ((2.0 * 4.0) / 5.0)");
	CHECK(Formula("2+((((((2*4)/5))").toString() == "2.0 + ((((((2.0 * 4.0) / 5.0)))))");
	CHECK(Formula("2+((2*4)/5))))))").toString() == "2.0 + ((2.0 * 4.0) / 5.0)");
	CHECK(Formula("1 + 2 - 3 * 4 / 5 % 6 :mod 7 ^ 1 :pow 2").toString() == "1.0 + 2.0 - 3.0 * 4.0 / 5.0 % 6.0 % 7.0 ^ 1.0 ^ 2.0");
	CHECK(Formula("1 :min 2 :max 3 :zero 4 :nvl 5 neg 6 :negz 7").toString() == "1.0 :min 2.0 :max 3.0 :nvl 4.0 :nvl 5.0 neg 6.0 :negz 7.0");
	CHECK(Formula("1 :pos 2 :posz 3 :rnd 4 :rndf 5 :rndn 6 :abs 7").toString() == "1.0 :pos 2.0 :posz 3.0 :rnd 4.0 :rndf 5.0 :rndn 6.0 :abs 7.0");
	CHECK(Formula("1 :ceil 2 :floor 3 :trunc 4 :round 5 :log 6 :ln 7").toString() == "1.0 :ceil 2.0 :floor 3.0 :trunc 4.0 :round 5.0 :log 6.0 :ln 7.0");
	CHECK(Formula("1 :sqrt 2 :cos 3 :sin 4 :tan 5").toString() == "1.0 :sqrt 2.0 :cos 3.0 :sin 4.0 :tan 5.0");
}
