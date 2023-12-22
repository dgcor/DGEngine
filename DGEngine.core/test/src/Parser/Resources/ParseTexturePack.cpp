#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseTexturePack", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/texturePacks.json");

	// imageContainer
	// existing id (doesn't override first resource parsed with the same id)
	auto texturePack = game.Resources().getTexturePack("key01");

	CHECK(texturePack != nullptr);
	CHECK(texturePack->size() == 4);

	// fromId
	texturePack = game.Resources().getTexturePack("key01");
	auto texturePack2 = game.Resources().getTexturePack("key02");

	CHECK(texturePack != nullptr);
	CHECK(texturePack == texturePack2);
}
