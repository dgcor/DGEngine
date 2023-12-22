#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexture", "Parser")
{
	Game game(true);
	Parser::parseFile(game, "json/textures.json");

	// invalid id from file
	auto texture = game.Resources().getTexture("invalid-id");

	CHECK(texture == nullptr);

	// id from file + split into 1 piece (no split)
	// existing id (doesn't override first resource parsed with the same id)
	texture = game.Resources().getTexture("texture");
	auto img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(16, 8));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);
	CHECK(img.getPixel(1, 0) == sf::Color::Red);
	CHECK(img.getPixel(0, 1) == sf::Color::Red);
	CHECK(img.getPixel(1, 1) == sf::Color::Red);

	// file + horizontal split into 2 pieces
	texture = game.Resources().getTexture("key01");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(8, 16));

	// file + vertical split into 2 pieces
	texture = game.Resources().getTexture("key02");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(32, 4));

	// file + VERTICAL split into 2 pieces
	texture = game.Resources().getTexture("key03");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(16, 8));

	// file + invalid split into 2 pieces
	texture = game.Resources().getTexture("key04");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(16, 8));

	// file + trim + mask
	texture = game.Resources().getTexture("key05");
	img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(2, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Transparent);
	CHECK(img.getPixel(1, 0) == sf::Color::Transparent);
	CHECK(img.getPixel(0, 1) == sf::Color::Transparent);
	CHECK(img.getPixel(1, 1) == sf::Color::Transparent);

	// single color
	texture = game.Resources().getTexture("key06");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(2, 2));

	// color array
	texture = game.Resources().getTexture("key07");
	img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(2, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);
	CHECK(img.getPixel(1, 0) == sf::Color::Green);
	CHECK(img.getPixel(0, 1) == sf::Color::Blue);
	CHECK(img.getPixel(1, 1) == sf::Color::White);

	// single color + no size
	texture = game.Resources().getTexture("key08");

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(640, 480));

	// color array + wrong size
	texture = game.Resources().getTexture("key09");
	img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(1, 1));
	CHECK(img.getPixel(0, 0) == sf::Color::Transparent);

	// color array + trim
	texture = game.Resources().getTexture("key10");
	img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(2, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Blue);
	CHECK(img.getPixel(1, 0) == sf::Color::Blue);
	CHECK(img.getPixel(0, 1) == sf::Color::Blue);
	CHECK(img.getPixel(1, 1) == sf::Color::Blue);

	// color array (array colors)
	texture = game.Resources().getTexture("key11");
	img = texture->copyToImage();

	CHECK(texture != nullptr);
	CHECK(texture->getSize() == sf::Vector2u(2, 2));
	CHECK(img.getPixel(0, 0) == sf::Color::Red);
	CHECK(img.getPixel(1, 0) == sf::Color::Green);
	CHECK(img.getPixel(0, 1) == sf::Color::Blue);
	CHECK(img.getPixel(1, 1) == sf::Color::Black);

	// fromId
	texture = game.Resources().getTexture("texture");
	auto texture2 = game.Resources().getTexture("key12");

	CHECK(texture != nullptr);
	CHECK(texture == texture2);
}
