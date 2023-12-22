#include "catch2/catch_test_macros.hpp"

#include "Game/Utils/VarUtils.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

TEST_CASE("VarUtils::toBool", "GameUtils")
{
	SECTION("string")
	{
		// only false if string is empty
		CHECK(VarUtils::toBool("") == false);
		CHECK(VarUtils::toBool("true") == true);
		CHECK(VarUtils::toBool("false") == true);
		CHECK(VarUtils::toBool("0") == true);
		CHECK(VarUtils::toBool("0.1") == true);
		CHECK(VarUtils::toBool("-0") == true);
		CHECK(VarUtils::toBool("1") == true);
		CHECK(VarUtils::toBool("1.5") == true);
		CHECK(VarUtils::toBool("-1.5") == true);
	}
	SECTION("int")
	{
		CHECK(VarUtils::toBool(0) == false);
		CHECK(VarUtils::toBool(1) == true);
		CHECK(VarUtils::toBool(1200) == true);
		CHECK(VarUtils::toBool(-99) == true);
	}
	SECTION("double")
	{
		CHECK(VarUtils::toBool(0.0) == false);
		CHECK(VarUtils::toBool(0.1) == true);
		CHECK(VarUtils::toBool(1.5) == true);
		CHECK(VarUtils::toBool(-2.999) == true);
	}
	SECTION("bool")
	{
		CHECK(VarUtils::toBool(true) == true);
		CHECK(VarUtils::toBool(false) == false);
	}
	SECTION("tuples")
	{
		CHECK(VarUtils::toBool(Int64Tuple2(1, -2)) == false);
		CHECK(VarUtils::toBool(Int64Tuple3(1, -2, 0)) == false);
		CHECK(VarUtils::toBool(Int64Tuple4(1, -2, 0, 999)) == false);
		CHECK(VarUtils::toBool(DoubleTuple2(1.49, -2.1)) == false);
		CHECK(VarUtils::toBool(DoubleTuple3(1.49, -2.1, 0.0)) == false);
		CHECK(VarUtils::toBool(DoubleTuple4(1.49, -2.1, 0.0, 999.999)) == false);
	}
}

TEST_CASE("VarUtils::toDouble", "GameUtils")
{
	SECTION("string")
	{
		CHECK(VarUtils::toDouble("") == 0.0);
		CHECK(VarUtils::toDouble("true") == 0.0);
		CHECK(VarUtils::toDouble("false") == 0.0);
		CHECK(VarUtils::toDouble("0") == 0.0);
		CHECK(VarUtils::toDouble("0.1") == 0.1);
		CHECK(VarUtils::toDouble("-0") == 0.0);
		CHECK(VarUtils::toDouble("1") == 1.0);
		CHECK(VarUtils::toDouble("1.5") == 1.5);
		CHECK(VarUtils::toDouble("-1.5") == -1.5);
	}
	SECTION("int")
	{
		CHECK(VarUtils::toDouble(0) == 0.0);
		CHECK(VarUtils::toDouble(1) == 1.0);
		CHECK(VarUtils::toDouble(1200) == 1200.0);
		CHECK(VarUtils::toDouble(-99) == -99.0);
	}
	SECTION("double")
	{
		CHECK(VarUtils::toDouble(0.0) == 0.0);
		CHECK(VarUtils::toDouble(0.1) == 0.1);
		CHECK(VarUtils::toDouble(1.5) == 1.5);
		CHECK(VarUtils::toDouble(-2.999) == -2.999);
	}
	SECTION("bool")
	{
		CHECK(VarUtils::toDouble(true) == 1.0);
		CHECK(VarUtils::toDouble(false) == 0.0);
	}
	SECTION("tuples")
	{
		CHECK(VarUtils::toDouble(Int64Tuple2(1, -2)) == 0.0);
		CHECK(VarUtils::toDouble(Int64Tuple3(1, -2, 0)) == 0.0);
		CHECK(VarUtils::toDouble(Int64Tuple4(1, -2, 0, 999)) == 0.0);
		CHECK(VarUtils::toDouble(DoubleTuple2(1.49, -2.1)) == 0.0);
		CHECK(VarUtils::toDouble(DoubleTuple3(1.49, -2.1, 0.0)) == 0.0);
		CHECK(VarUtils::toDouble(DoubleTuple4(1.49, -2.1, 0.0, 999.999)) == 0.0);
	}
}

TEST_CASE("VarUtils::toInt64", "GameUtils")
{
	SECTION("string")
	{
		CHECK(VarUtils::toInt64("") == 0);
		CHECK(VarUtils::toInt64("true") == 0);
		CHECK(VarUtils::toInt64("false") == 0);
		CHECK(VarUtils::toInt64("0") == 0);
		CHECK(VarUtils::toInt64("0.1") == 0);
		CHECK(VarUtils::toInt64("-0") == 0);
		CHECK(VarUtils::toInt64("1") == 1);
		CHECK(VarUtils::toInt64("1.5") == 1);
		CHECK(VarUtils::toInt64("-1.5") == -1);
	}
	SECTION("int")
	{
		CHECK(VarUtils::toInt64(0) == 0);
		CHECK(VarUtils::toInt64(1) == 1);
		CHECK(VarUtils::toInt64(1200) == 1200);
		CHECK(VarUtils::toInt64(-99) == -99);
	}
	SECTION("double")
	{
		CHECK(VarUtils::toInt64(0.0) == 0);
		CHECK(VarUtils::toInt64(0.1) == 0);
		CHECK(VarUtils::toInt64(1.5) == 1);
		CHECK(VarUtils::toInt64(-2.999) == -2);
	}
	SECTION("bool")
	{
		CHECK(VarUtils::toInt64(true) == 1);
		CHECK(VarUtils::toInt64(false) == 0);
	}
	SECTION("tuples")
	{
		CHECK(VarUtils::toInt64(Int64Tuple2(1, -2)) == 0);
		CHECK(VarUtils::toInt64(Int64Tuple3(1, -2, 0)) == 0);
		CHECK(VarUtils::toInt64(Int64Tuple4(1, -2, 0, 999)) == 0);
		CHECK(VarUtils::toInt64(DoubleTuple2(1.49, -2.1)) == 0);
		CHECK(VarUtils::toInt64(DoubleTuple3(1.49, -2.1, 0.0)) == 0);
		CHECK(VarUtils::toInt64(DoubleTuple4(1.49, -2.1, 0.0, 999.999)) == 0);
	}
}

TEST_CASE("VarUtils::toString", "GameUtils")
{
	SECTION("string")
	{
		CHECK(VarUtils::toString("") == "");
		CHECK(VarUtils::toString("true") == "true");
		CHECK(VarUtils::toString("false") == "false");
		CHECK(VarUtils::toString("0") == "0");
		CHECK(VarUtils::toString("0.1") == "0.1");
		CHECK(VarUtils::toString("-0") == "-0");
		CHECK(VarUtils::toString("1") == "1");
		CHECK(VarUtils::toString("1.5") == "1.5");
		CHECK(VarUtils::toString("-1.5") == "-1.5");
	}
	SECTION("int")
	{
		CHECK(VarUtils::toString(0) == "0");
		CHECK(VarUtils::toString(1) == "1");
		CHECK(VarUtils::toString(1200) == "1200");
		CHECK(VarUtils::toString(-99) == "-99");
	}
	SECTION("double")
	{
		CHECK(VarUtils::toString(0.0) == "0.0");
		CHECK(VarUtils::toString(0.1) == "0.1");
		CHECK(VarUtils::toString(1.5) == "1.5");
		CHECK(VarUtils::toString(-2.999) == "-2.999");
	}
	SECTION("bool")
	{
		CHECK(VarUtils::toString(true) == "true");
		CHECK(VarUtils::toString(false) == "false");
	}
	SECTION("tuples")
	{
		CHECK(VarUtils::toString(Int64Tuple2(1, -2)) == "[1, -2]");
		CHECK(VarUtils::toString(Int64Tuple3(1, -2, 0)) == "[1, -2, 0]");
		CHECK(VarUtils::toString(Int64Tuple4(1, -2, 0, 999)) == "[1, -2, 0, 999]");
		CHECK(VarUtils::toString(DoubleTuple2(1.49, -2.1)) == "[1.49, -2.1]");
		CHECK(VarUtils::toString(DoubleTuple3(1.49, -2.1, 0.0)) == "[1.49, -2.1, 0.0]");
		CHECK(VarUtils::toString(DoubleTuple4(1.49, -2.1, 0.0, 999.999)) == "[1.49, -2.1, 0.0, 999.999]");
	}
}

TEST_CASE("VarUtils::getVector", "GameUtils")
{
	SECTION("vector2 float")
	{
		Variable var = DoubleTuple2(10.f, -20.f);

		CHECK(VarUtils::getVector2fVal<sf::Vector2f>(var) == sf::Vector2f(10.f, -20.f));
		CHECK(VarUtils::getVector2fVal<sf::Vector2f>("text") == sf::Vector2f());
	}
	SECTION("vector2 int32_t")
	{
		Variable var = Int64Tuple2(10, -20);

		CHECK(VarUtils::getVector2iVal<sf::Vector2i>(var) == sf::Vector2i(10, -20));
		CHECK(VarUtils::getVector2iVal<sf::Vector2i>("text") == sf::Vector2i());
	}
	SECTION("vector2 uint32_t")
	{
		Variable var = Int64Tuple2(10, -20);

		CHECK(VarUtils::getVector2uVal<sf::Vector2u>(var) == sf::Vector2u(10, -20));
		CHECK(VarUtils::getVector2uVal<sf::Vector2u>("text") == sf::Vector2u());
	}
	SECTION("vector2 int64_t")
	{
		using VectorType = std::tuple<int64_t, int64_t>;
		Variable var = Int64Tuple2(10, -20);

		CHECK(VarUtils::getVector2NumberVal<VectorType, int64_t>(var) == VectorType(10, -20));
		CHECK(VarUtils::getVector2NumberVal<VectorType, int64_t>("text") == VectorType());
	}
	SECTION("vector3 float")
	{
		Variable var = DoubleTuple3(10.f, -20.f, 30.f);

		CHECK(VarUtils::getVector3fVal<sf::Vector3f>(var) == sf::Vector3f(10.f, -20.f, 30.f));
		CHECK(VarUtils::getVector3fVal<sf::Vector3f>("text") == sf::Vector3f());
	}
	SECTION("vector3 int32_t")
	{
		Variable var = Int64Tuple3(10, -20, 30);

		CHECK(VarUtils::getVector3iVal<sf::Vector3i>(var) == sf::Vector3i(10, -20, 30));
		CHECK(VarUtils::getVector3iVal<sf::Vector3i>("text") == sf::Vector3i());
	}
	SECTION("vector3 uint32_t")
	{
		using VectorType = std::tuple<uint32_t, uint32_t, uint32_t>;
		Variable var = Int64Tuple3(10, -20, 30);

		CHECK(VarUtils::getVector3uVal<VectorType>(var) == VectorType(10, -20, 30));
		CHECK(VarUtils::getVector3uVal<VectorType>("text") == VectorType());
	}
	SECTION("vector3 int64_t")
	{
		using VectorType = std::tuple<int64_t, int64_t, int64_t>;
		Variable var = Int64Tuple3(10, -20, 30);

		CHECK(VarUtils::getVector3NumberVal<VectorType, int64_t>(var) == VectorType(10, -20, 30));
		CHECK(VarUtils::getVector3NumberVal<VectorType, int64_t>("text") == VectorType());
	}
	SECTION("vector4 float")
	{
		Variable var = DoubleTuple4(10.f, -20.f, 30.f, 0.f);

		CHECK(VarUtils::getVector4fVal<sf::FloatRect>(var) == sf::FloatRect(10.f, -20.f, 30.f, 0.f));
		CHECK(VarUtils::getVector4fVal<sf::FloatRect>("text") == sf::FloatRect());
	}
	SECTION("vector4 int32_t")
	{
		Variable var = Int64Tuple4(10, -20, 30, 0);

		CHECK(VarUtils::getVector4iVal<sf::IntRect>(var) == sf::IntRect(10, -20, 30, 0));
		CHECK(VarUtils::getVector4iVal<sf::IntRect>("text") == sf::IntRect());
	}
	SECTION("vector4 uint32_t")
	{
		using VectorType = std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>;
		Variable var = Int64Tuple4(10, -20, 30, 0);

		CHECK(VarUtils::getVector4uVal<VectorType>(var) == VectorType(10, -20, 30, 0));
		CHECK(VarUtils::getVector4uVal<VectorType>("text") == VectorType());
	}
	SECTION("vector4 int64_t")
	{
		using VectorType = std::tuple<int64_t, int64_t, int64_t, int64_t>;
		Variable var = Int64Tuple4(10, -20, 30, 0);

		CHECK(VarUtils::getVector4NumberVal<VectorType, int64_t>(var) == VectorType(10, -20, 30, 0));
		CHECK(VarUtils::getVector4NumberVal<VectorType, int64_t>("text") == VectorType());
	}
}
