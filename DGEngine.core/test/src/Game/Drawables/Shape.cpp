#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/Shape.h"
#include "TestUtils/GameFixture.h"

TEST_CASE("Shape::constructor", "Shape")
{
	SECTION("default")
	{
		Shape obj(sf::PrimitiveType::TriangleStrip);

		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.Size() == sf::Vector2f(0.f, 0.f));
		CHECK(obj.Visible() == true);
	}
}

TEST_CASE("Shape::getProperty", "Shape")
{
	Shape obj(sf::PrimitiveType::TriangleStrip);
	Variable var;

	obj.addVertex(sf::Vertex(sf::Vector2f(0.f, 0.f)));
	obj.addVertex(sf::Vertex(sf::Vector2f(2.f, 2.f)));
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
		CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(2, 2)));

		CHECK(obj.getProperty("size.x", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 2));

		CHECK(obj.getProperty("size.y", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 2));

		CHECK(obj.getProperty("visible", var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));
	}
	SECTION("invalid")
	{
		CHECK(obj.getProperty("invalid", var) == false);
	}
}

TEST_CASE("Shape::updateSize", "Shape")
{
	GameFixture game(true);
	auto obj = std::make_shared<Shape>(sf::PrimitiveType::TriangleStrip);
	obj->addVertex(sf::Vertex(sf::Vector2f(0.f, 0.f)));
	obj->addVertex(sf::Vertex(sf::Vector2f(2.f, 2.f)));
	game.Resources().addDrawable("obj", obj, true);

	SECTION("position")
	{
		obj->setAnchor(Anchor::Right);
		obj->Position({10, 20});

		game.updateSize({800, 600});
		obj->updateSize(game);

		CHECK(obj->Position() == sf::Vector2f(170.f, 80.f));
		CHECK(obj->Size() == sf::Vector2f(2.f, 2.f));
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
		CHECK(obj->Size() == sf::Vector2f(162.f, 122.f));
	}
}
