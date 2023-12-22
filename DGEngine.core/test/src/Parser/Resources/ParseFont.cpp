#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseFont", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/fonts.json");

	// invalid id from file
	auto font = game.Resources().getFont("LiberationSerif-Regular");

	CHECK(holdsNullFont(font) == true);

	// id from file
	// existing id (doesn't override first resource parsed with the same id)
	font = game.Resources().getFont("font");
	std::shared_ptr<FreeTypeFont> freeTypeFont;

	CHECK(holdsFreeTypeFont(font) == true);
	freeTypeFont = std::get<std::shared_ptr<FreeTypeFont>>(font);
	CHECK(freeTypeFont->getColor() == sf::Color::White);
	CHECK(freeTypeFont->getCharacterSize() == 0);
	CHECK(freeTypeFont->isSmooth() == true);

	// file + color + size + smooth
	font = game.Resources().getFont("key01");

	CHECK(holdsFreeTypeFont(font) == true);
	freeTypeFont = std::get<std::shared_ptr<FreeTypeFont>>(font);
	CHECK(freeTypeFont->getColor() == sf::Color::Red);
	CHECK(freeTypeFont->getCharacterSize() == 14);
	CHECK(freeTypeFont->isSmooth() == false);

	// clone
	auto font2 = game.Resources().getFont("key02");

	CHECK(font != font2);
	CHECK(holdsFreeTypeFont(font) == true);

	// clone + color + size + smooth (not updated)
	font = game.Resources().getFont("key03");

	CHECK(holdsFreeTypeFont(font) == true);
	freeTypeFont = std::get<std::shared_ptr<FreeTypeFont>>(font);
	CHECK(freeTypeFont->getColor() == sf::Color::Blue);
	CHECK(freeTypeFont->getCharacterSize() == 16);
	CHECK(freeTypeFont->isSmooth() == true);

	// fromId
	font = game.Resources().getFont("font");
	font2 = game.Resources().getFont("key04");

	CHECK(font == font2);
}
