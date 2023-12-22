#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParsePalette", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/palettes.json");

	// invalid id from file
	auto palette = game.Resources().getPalette("invalid-id");

	// id from file
	palette = game.Resources().getPalette("pal");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Red);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Blue);

	// default binary
	palette = game.Resources().getPalette("key01");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Red);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Blue);

	// existing id
	auto palette2 = game.Resources().getPalette("key05");

	CHECK(palette != palette2);

	// default text
	palette = game.Resources().getPalette("key02");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Red);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Blue);

	// RGB
	palette = game.Resources().getPalette("key03");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Red);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Blue);

	// BGR
	palette = game.Resources().getPalette("key04");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Blue);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Red);

	// RGBA
	palette = game.Resources().getPalette("key05");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color(0xFF, 0x00, 0x00, 0xAA));
	CHECK((*palette)[1] == sf::Color(0x00, 0xFF, 0x00, 0xAA));
	CHECK((*palette)[2] == sf::Color(0x00, 0x00, 0xFF, 0xAA));

	// BGRA
	palette = game.Resources().getPalette("key06");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color(0x00, 0x00, 0xFF, 0xAA));
	CHECK((*palette)[1] == sf::Color(0x00, 0xFF, 0x00, 0xAA));
	CHECK((*palette)[2] == sf::Color(0xFF, 0x00, 0x00, 0xAA));

	// ARGB
	palette = game.Resources().getPalette("key07");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color(0x00, 0x00, 0xAA, 0xFF));
	CHECK((*palette)[1] == sf::Color(0xFF, 0x00, 0xAA, 0x00));
	CHECK((*palette)[2] == sf::Color(0x00, 0xFF, 0xAA, 0x00));

	// ABGR
	palette = game.Resources().getPalette("key08");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color(0xAA, 0x00, 0x00, 0xFF));
	CHECK((*palette)[1] == sf::Color(0xAA, 0x00, 0xFF, 0x00));
	CHECK((*palette)[2] == sf::Color(0xAA, 0xFF, 0x00, 0x00));

	// invalid data
	// gray palette when file is too small for 256 RGB colors
	palette = game.Resources().getPalette("key09");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color(0x00, 0x00, 0x00, 0xFF));
	CHECK((*palette)[1] == sf::Color(0x01, 0x01, 0x01, 0xFF));
	CHECK((*palette)[127] == sf::Color(0x7F, 0x7F, 0x7F, 0xFF));
	CHECK((*palette)[254] == sf::Color(0xFE, 0xFE, 0xFE, 0xFF));
	CHECK((*palette)[255] == sf::Color(0xFF, 0xFF, 0xFF, 0xFF));

	// trnFile
	palette = game.Resources().getPalette("key10");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Blue);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Red);

	// id from trnFile
	palette = game.Resources().getPalette("trn");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Blue);
	CHECK((*palette)[1] == sf::Color::Green);
	CHECK((*palette)[2] == sf::Color::Red);

	// trnStart + trnLength
	// black color when trn index not applied
	palette = game.Resources().getPalette("key11");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::White);
	CHECK((*palette)[1] == sf::Color::Black);
	CHECK((*palette)[2] == sf::Color::Black);

	// trnStart + trnLength out of range
	// black palette when trn index out of range
	palette = game.Resources().getPalette("key12");

	CHECK(palette != nullptr);
	CHECK((*palette)[0] == sf::Color::Black);
	CHECK((*palette)[255] == sf::Color::Black);

	// clone
	palette = game.Resources().getPalette("pal");
	palette2 = game.Resources().getPalette("key13");

	CHECK(palette != nullptr);
	CHECK(palette2 != nullptr);
	CHECK(palette != palette2);
	CHECK((*palette)[0] == (*palette2)[0]);
	CHECK((*palette)[1] == (*palette2)[1]);
	CHECK((*palette)[2] == (*palette2)[2]);
	CHECK((*palette)[255] == (*palette2)[255]);

	// fromId
	palette = game.Resources().getPalette("pal");
	palette2 = game.Resources().getPalette("key14");

	CHECK(palette != nullptr);
	CHECK(palette == palette2);
}
