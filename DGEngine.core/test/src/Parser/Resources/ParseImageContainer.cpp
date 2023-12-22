#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseimageContainer", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/imageContainers.json");

	// invalid id from file
	auto imageContainer = game.Resources().getImageContainer("invalid-id");

	CHECK(imageContainer == nullptr);

	// id from file + 1 frame
	// existing id (doesn't override first resource parsed with the same id)
	imageContainer = game.Resources().getImageContainer("imageContainer");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 1);
	CHECK(imageContainer->getBlendMode() == BlendMode::Alpha);
	CHECK(imageContainer->getDirections() == 1);
	CHECK(imageContainer->getImageSize(0) == sf::Vector2u(16, 8));

	// id from file + 0 frames
	imageContainer = game.Resources().getImageContainer("key01");

	CHECK(imageContainer == nullptr);

	// frame range (horizontal direction) + blendMode
	imageContainer = game.Resources().getImageContainer("key02");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);
	CHECK(imageContainer->getBlendMode() == BlendMode::Add);
	CHECK(imageContainer->getDirections() == 1);

	auto img = imageContainer->get(0, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(2, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(3, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	// frame range + vertical direction + 4 directions
	imageContainer = game.Resources().getImageContainer("key03");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);
	CHECK(imageContainer->getDirections() == 4);

	img = imageContainer->get(0, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(2, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(3, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	// frame range + VERTICAL direction + odd directions
	imageContainer = game.Resources().getImageContainer("key04");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);
	CHECK(imageContainer->getDirections() == 3);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	// frame range + directions > frames
	imageContainer = game.Resources().getImageContainer("key05");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);
	CHECK(imageContainer->getDirections() == 8);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	// frame range + 2 directions
	imageContainer = game.Resources().getImageContainer("key06");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);
	CHECK(imageContainer->getDirections() == 2);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	// odd frame range + horizontal direction
	imageContainer = game.Resources().getImageContainer("key07");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 8);
	CHECK(imageContainer->getDirections() == 1);

	img = imageContainer->get(0, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(2, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(3, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(4, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(5, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(6, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	img = imageContainer->get(7, nullptr);

	CHECK(img.getSize() == sf::Vector2u(4, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	// odd frame range + vertical direction
	imageContainer = game.Resources().getImageContainer("key08");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 8);
	CHECK(imageContainer->getDirections() == 1);

	img = imageContainer->get(0, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(1, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(2, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(3, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(4, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(5, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(6, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	img = imageContainer->get(7, nullptr);

	CHECK(img.getSize() == sf::Vector2u(8, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	// invalid frame range (non integer split)
	imageContainer = game.Resources().getImageContainer("key09");

	CHECK(imageContainer == nullptr);

	// frame range + vertical direction + palette image
	imageContainer = game.Resources().getImageContainer("key10");
	auto pal = game.Resources().getPalette("pal");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer->size() == 4);

	img = imageContainer->get(0, &pal->palette);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);

	img = imageContainer->get(1, &pal->palette);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);

	img = imageContainer->get(2, &pal->palette);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::Green);

	img = imageContainer->get(3, &pal->palette);

	CHECK(img.getSize() == sf::Vector2u(8, 4));
	CHECK(img.getPixel(0, 0) == sf::Color::White);

	// fromId
	imageContainer = game.Resources().getImageContainer("imageContainer");
	auto imageContainer2 = game.Resources().getImageContainer("key11");

	CHECK(imageContainer != nullptr);
	CHECK(imageContainer == imageContainer2);
}
