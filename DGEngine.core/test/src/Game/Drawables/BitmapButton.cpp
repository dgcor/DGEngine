#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/BitmapButton.h"
#include "TestUtils/GameFixture.h"
#include "TestUtils/MakeButton.h"
#include "TestUtils/MakeTexture.h"

TEST_CASE("BitmapButton::constructor", "BitmapButton")
{
	SECTION("default")
	{
		BitmapButton obj;

		CHECK(obj.Background() == false);
		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getColor() == sf::Color::White);
		CHECK(obj.getTexture() == nullptr);
		CHECK(obj.hasPalette() == false);
		CHECK(obj.isOutlineEnabled() == true);
		CHECK(obj.Resizable() == false);
		CHECK(obj.Size() == sf::Vector2f(0.f, 0.f));
		CHECK(obj.Stretch() == false);
		CHECK(obj.Visible() == true);
	}
	SECTION("texture")
	{
		auto tex = makeTexture();
		BitmapButton obj(*tex);

		CHECK(obj.Background() == false);
		CHECK(obj.getAnchor() == (Anchor::Top | Anchor::Left));
		CHECK(obj.getColor() == sf::Color::White);
		CHECK(obj.getTexture() != nullptr);
		CHECK(obj.hasPalette() == false);
		CHECK(obj.isOutlineEnabled() == true);
		CHECK(obj.Resizable() == false);
		CHECK(obj.Size() == sf::Vector2f(16.f, 8.f));
		CHECK(obj.Stretch() == false);
		CHECK(obj.Visible() == true);
	}
}

TEST_CASE("BitmapButton::getProperty", "BitmapButton")
{
	auto tex = makeTexture();
	BitmapButton obj(*tex);
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
		CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(16, 8)));

		CHECK(obj.getProperty("size.x", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 16));

		CHECK(obj.getProperty("size.y", var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 8));

		CHECK(obj.getProperty("visible", var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));
	}
	SECTION("Button")
	{
		CHECK(obj.getProperty("clickUp", var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

		CHECK(obj.getProperty("enabled", var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));
	}
	SECTION("invalid")
	{
		CHECK(obj.getProperty("invalid", var) == false);
	}
}

TEST_CASE("BitmapButton::updateSize", "BitmapButton")
{
	GameFixture game(true);
	auto obj = makeBitmapButton(game, "obj", makeTexture(game, "tex"));

	SECTION("position")
	{
		obj->setAnchor(Anchor::Right);
		obj->Position({10, 20});

		game.updateSize({800, 600});
		obj->updateSize(game);

		CHECK(obj->Position() == sf::Vector2f(170.f, 80.f));
		CHECK(obj->Size() == sf::Vector2f(16.f, 8.f));
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
		CHECK(obj->Size() == sf::Vector2f(16.f, 8.f));
	}
	SECTION("position + size + resizable")
	{
		game.updateSize({640, 480});
		obj->updateSize(game);

		obj->setAnchor(Anchor::All);
		obj->Resizable(true);
		obj->Position({10, 20});

		game.updateSize({800, 600});
		obj->updateSize(game);

		CHECK(obj->Position() == sf::Vector2f(10.f, 20.f));
		CHECK(obj->Size() == sf::Vector2f(176.f, 128.f));
	}
}
