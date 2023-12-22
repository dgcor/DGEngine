#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Game/Utils/ImageUtils.h"
#include "Parser/ParseFile.h"

TEST_CASE("ImageUtils::applyMask", "ImageUtils")
{
	sf::Image img;
	img.create(2, 2, sf::Color::Red);

	SECTION("no update")
	{
		ImageUtils::applyMask(img);

		CHECK(img.getPixel(0, 0) == sf::Color::Red);
		CHECK(img.getPixel(1, 0) == sf::Color::Red);
		CHECK(img.getPixel(0, 1) == sf::Color::Red);
		CHECK(img.getPixel(1, 1) == sf::Color::Red);
	}
	SECTION("update")
	{
		img.setPixel(0, 0, sf::Color::Green);
		img.setPixel(1, 1, sf::Color::Green);

		ImageUtils::applyMask(img, sf::Color::Green);

		CHECK(img.getPixel(0, 0) == sf::Color::Transparent);
		CHECK(img.getPixel(1, 0) == sf::Color::Red);
		CHECK(img.getPixel(0, 1) == sf::Color::Red);
		CHECK(img.getPixel(1, 1) == sf::Color::Transparent);
	}
}

TEST_CASE("ImageUtils::packImages", "ImageUtils")
{
	std::string json{ R"(
	{
	  "imageContainer": [
	    {
	      "id": "key01",
	      "file": "res/texture/imageContainer.png",
	      "frames": [2, 2],
	      "blendMode": "add"
	    }
	  ]
	})"
	};

	Game game;
	Parser::parseJson(game, json);

	auto imgContainer = game.Resources().getImageContainer("key01");

	auto img = ImageUtils::packImages(*imgContainer, nullptr, {});

	CHECK(img.getSize() == sf::Vector2u(8, 16));

	std::vector<sf::IntRect> rects;

	img = ImageUtils::packImages(*imgContainer, nullptr, &rects);

	CHECK(img.getSize() == sf::Vector2u(8, 16));
	CHECK(rects.size() == 4);
	CHECK(rects[0] == sf::IntRect(0, 0, 8, 4));
	CHECK(rects[1] == sf::IntRect(0, 4, 8, 4));
	CHECK(rects[2] == sf::IntRect(0, 8, 8, 4));
	CHECK(rects[3] == sf::IntRect(0, 12, 8, 4));
}

TEST_CASE("ImageUtils::splitImageHorizontal", "ImageUtils")
{
	sf::Image img;
	img.create(2, 2, sf::Color::Red);
	img.setPixel(0, 0, sf::Color::Green);
	img.setPixel(1, 1, sf::Color::Green);

	SECTION("no update")
	{
		auto image = ImageUtils::splitImageHorizontal(img, 1);

		CHECK(image.getSize() == sf::Vector2u(2, 2));
		CHECK(image.getPixel(0, 0) == sf::Color::Green);
		CHECK(image.getPixel(1, 0) == sf::Color::Red);
		CHECK(image.getPixel(0, 1) == sf::Color::Red);
		CHECK(image.getPixel(1, 1) == sf::Color::Green);
	}
	SECTION("uneven split")
	{
		sf::Image img2;
		img2.create(3, 3, sf::Color::Red);

		auto image = ImageUtils::splitImageHorizontal(img2, 2);

		CHECK(image.getSize() == sf::Vector2u(3, 3));
	}
	SECTION("update")
	{
		auto image = ImageUtils::splitImageHorizontal(img, 2);

		CHECK(image.getSize() == sf::Vector2u(1, 4));
		CHECK(image.getPixel(0, 0) == sf::Color::Green);
		CHECK(image.getPixel(0, 1) == sf::Color::Red);
		CHECK(image.getPixel(0, 2) == sf::Color::Red);
		CHECK(image.getPixel(0, 3) == sf::Color::Green);
	}
}

TEST_CASE("ImageUtils::splitImageVertical", "ImageUtils")
{
	sf::Image img;
	img.create(2, 2, sf::Color::Red);
	img.setPixel(0, 0, sf::Color::Green);
	img.setPixel(1, 1, sf::Color::Green);

	SECTION("no update")
	{
		auto image = ImageUtils::splitImageVertical(img, 1);

		CHECK(image.getSize() == sf::Vector2u(2, 2));
		CHECK(image.getPixel(0, 0) == sf::Color::Green);
		CHECK(image.getPixel(1, 0) == sf::Color::Red);
		CHECK(image.getPixel(0, 1) == sf::Color::Red);
		CHECK(image.getPixel(1, 1) == sf::Color::Green);
	}
	SECTION("uneven split")
	{
		sf::Image img2;
		img2.create(3, 3, sf::Color::Red);

		auto image = ImageUtils::splitImageVertical(img2, 2);

		CHECK(image.getSize() == sf::Vector2u(3, 3));
	}
	SECTION("update")
	{
		auto image = ImageUtils::splitImageVertical(img, 2);

		CHECK(image.getSize() == sf::Vector2u(4, 1));
		CHECK(image.getPixel(0, 0) == sf::Color::Green);
		CHECK(image.getPixel(1, 0) == sf::Color::Red);
		CHECK(image.getPixel(2, 0) == sf::Color::Red);
		CHECK(image.getPixel(3, 0) == sf::Color::Green);
	}
}
