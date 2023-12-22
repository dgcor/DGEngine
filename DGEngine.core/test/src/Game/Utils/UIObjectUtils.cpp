#include "catch2/catch_test_macros.hpp"

#include "Game/Utils/UIObjectUtils.h"
#include <SFML/System/Vector2.hpp>
#include "Utils/PairXY.h"

TEST_CASE("UIObjectUtils::getTuple2Prop", "UIObjectUtils")
{
	SECTION("float")
	{
		CHECK(UIObjectUtils::getTuple2fProp(sf::Vector2f(1.f, 2.f), "") == Variable(DoubleTuple2(1.f, 2.f)));
		CHECK(UIObjectUtils::getTuple2fProp(sf::Vector2f(1.f, 2.f), "x") == Variable(1.f));
		CHECK(UIObjectUtils::getTuple2fProp(sf::Vector2f(1.f, 2.f), "y") == Variable(2.f));
		CHECK(UIObjectUtils::getTuple2fProp(sf::Vector2f(1.f, 2.f), "invalid") == Variable(2.f));
	}
	SECTION("int32_t")
	{
		CHECK(UIObjectUtils::getTuple2iProp(sf::Vector2i(1, 2), "") == Variable(Int64Tuple2(1, 2)));
		CHECK(UIObjectUtils::getTuple2iProp(sf::Vector2i(1, 2), "x") == Variable(1));
		CHECK(UIObjectUtils::getTuple2iProp(sf::Vector2i(1, 2), "y") == Variable(2));
		CHECK(UIObjectUtils::getTuple2iProp(sf::Vector2i(1, 2), "invalid") == Variable(2));
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = PairXY<Type>;

		CHECK(UIObjectUtils::getTuple2Prop<VectorType, Type>(VectorType(1, 2), "") == Variable(Int64Tuple2(1, 2)));
		CHECK(UIObjectUtils::getTuple2Prop<VectorType, Type>(VectorType(1, 2), "x") == Variable(1));
		CHECK(UIObjectUtils::getTuple2Prop<VectorType, Type>(VectorType(1, 2), "y") == Variable(2));
		CHECK(UIObjectUtils::getTuple2Prop<VectorType, Type>(VectorType(1, 2), "invalid") == Variable(2));
	}
}
