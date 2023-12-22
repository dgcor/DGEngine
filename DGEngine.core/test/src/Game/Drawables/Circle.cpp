#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/Circle.h"
#include "TestUtils/GameFixture.h"

TEST_CASE("Circle::constructor", "Circle")
{
	SECTION("default")
	{
		Circle obj;

		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getPointCount() == 30);
		CHECK(obj.getRadius() == 0.f);
		CHECK(obj.Size() == sf::Vector2f(0.f, 0.f));
		CHECK(obj.Visible() == true);
	}
	SECTION("radius")
	{
		Circle obj(2.f);

		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getPointCount() == 30);
		CHECK(obj.getRadius() == 2.f);
		CHECK(obj.Size() == sf::Vector2f(4.f, 4.f));
		CHECK(obj.Visible() == true);
	}
	SECTION("radius + pointCount")
	{
		Circle obj(2.f, 40);

		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getPointCount() == 40);
		CHECK(obj.getRadius() == 2.f);
		CHECK(obj.Visible() == true);
	}
	SECTION("fractional radius")
	{
		// radius gets rounded
		Circle obj(1.5f);

		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getPointCount() == 30);
		CHECK(obj.getRadius() == 2.f);
		CHECK(obj.Size() == sf::Vector2f(4.f, 4.f));
		CHECK(obj.Visible() == true);
	}
}

TEST_CASE("Circle::getProperty", "Circle")
{
	Circle obj(2.f);
	Variable var;

	obj.Position({10, 20});

	SECTION("UIObject")
	{
		CHECK(obj.getProperty("position", var) == true);
		CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(10, 20)));

		CHECK(obj.getProperty("position.x", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 10));

		CHECK(obj.getProperty("position.y", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 20));

		CHECK(obj.getProperty("size", var) == true);
		CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(4, 4)));

		CHECK(obj.getProperty("size.x", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 4));

		CHECK(obj.getProperty("size.y", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 4));

		CHECK(obj.getProperty("visible", var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));
	}
	SECTION("circle")
	{
		CHECK(obj.getProperty("points", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 30));

		CHECK(obj.getProperty("radius", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 2));
	}
	SECTION("invalid")
	{
		CHECK(obj.getProperty("invalid", var) == false);
	}
}

TEST_CASE("Circle::updateSize", "Circle")
{
	GameFixture game(true);
	auto obj = std::make_shared<Circle>(2.f);
	game.Resources().addDrawable("obj", obj, true);

	SECTION("position")
	{
		obj->setAnchor(Anchor::Right);
		obj->Position({10, 20});

		game.updateSize({800, 600});
		obj->updateSize(game);

		CHECK(obj->Position() == sf::Vector2f(170.f, 80.f));
		CHECK(obj->Size() == sf::Vector2f(4.f, 4.f));
	}
	SECTION("position + size")
	{
		game.updateSize({640, 480});
		obj->updateSize(game);

		obj->setAnchor(Anchor::All);
		obj->Position({10, 20});

		game.updateSize({800, 600});
		obj->updateSize(game);

		CHECK(obj->Position() == sf::Vector2f(10.f, 20.f));
		CHECK(obj->Size() == sf::Vector2f(4.f, 4.f));
	}
}
