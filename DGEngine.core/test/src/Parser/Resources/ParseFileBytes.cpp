#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseFileBytes", "Parser")
{
	Game game;
	Parser::parseFile(game, "json/files.json");

	// invalid id from file
	auto file = game.Resources().getFileBytes("invalid-id");
	CHECK(file == nullptr);

	// id from file
	file = game.Resources().getFileBytes("pal");

	CHECK(file != nullptr);
	CHECK(file->size() == 768);
	CHECK((*file)[0] == (std::byte)0xFF);
	CHECK((*file)[1] == (std::byte)0x00);
	CHECK((*file)[2] == (std::byte)0x00);

	// binary
	file = game.Resources().getFileBytes("key01");

	CHECK(file != nullptr);
	CHECK(file->size() == 768);
	CHECK((*file)[0] == (std::byte)0xFF);
	CHECK((*file)[1] == (std::byte)0x00);
	CHECK((*file)[2] == (std::byte)0x00);

	// startPosition + bytesToRead
	file = game.Resources().getFileBytes("key02");

	CHECK(file != nullptr);
	CHECK(file->size() == 2054);
	CHECK((*file)[0] == (std::byte)'2');
	CHECK((*file)[1] == (std::byte)'5');
	CHECK((*file)[2] == (std::byte)'5');

	// startPosition + bytesToRead
	file = game.Resources().getFileBytes("key03");

	CHECK(file != nullptr);
	CHECK(file->size() == 3);
	CHECK((*file)[0] == (std::byte)'5');
	CHECK((*file)[1] == (std::byte)'5');
	CHECK((*file)[2] == (std::byte)',');

	// startPosition + bytesToRead out of range
	file = game.Resources().getFileBytes("key04");

	CHECK(file != nullptr);
	CHECK(file->size() == 2051);
	CHECK((*file)[0] == (std::byte)',');
	CHECK((*file)[1] == (std::byte)' ');
	CHECK((*file)[2] == (std::byte)'0');

	// startPosition out of range + bytesToRead
	file = game.Resources().getFileBytes("key05");

	CHECK(file == nullptr);

	// no clone
	file = game.Resources().getFileBytes("key06");

	CHECK(file == nullptr);

	// fromId
	file = game.Resources().getFileBytes("pal");
	auto file2 = game.Resources().getFileBytes("key07");

	CHECK(file != nullptr);
	CHECK(file == file2);
}
