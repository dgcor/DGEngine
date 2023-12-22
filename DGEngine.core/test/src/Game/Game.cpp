#include "catch2/catch_test_macros.hpp"

#include "Game/Actions/ActGame.h"
#include "Game/Drawables/BitmapButton.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "TestUtils/MakeButton.h"
#include "TestUtils/MakeTexture.h"
#include "Utils/StringHash.h"

TEST_CASE("Game::constructor", "Game")
{
	SECTION("default")
	{
		Game game;

		CHECK(game.Size() == sf::Vector2u(0, 0));
	}
	SECTION("reset")
	{
		Game game(true);

		CHECK(game.Size() == sf::Vector2u(640, 480));
	}
}

TEST_CASE("Game::getGameProperty", "Game")
{
	Game game(true);
	Variable var;

	CHECK(game.getGameProperty("cursor", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2()));

	CHECK(game.getGameProperty("cursor.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("cursor.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("frameRate", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 60));

	CHECK(game.getGameProperty("fullScreen", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("gamma", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("hasAudio.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasAudio.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasDrawable.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasEvent.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasFont.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasGameShader", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getGameProperty("hasImageContainer.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasPalette.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasResource.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasShader.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasSong.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasSpriteShader", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getGameProperty("hasTexture.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("hasTexturePack.invalid", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("keepAR", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getGameProperty("maxHeight", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("minSize", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(640, 480)));

	CHECK(game.getGameProperty("minSize.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 640));

	CHECK(game.getGameProperty("minSize.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 480));

	CHECK(game.getGameProperty("musicVolume", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 100));

	CHECK(game.getGameProperty("openGL.antialiasingLevel", var) == true);
	CHECK(game.getGameProperty("openGL.depthBits", var) == true);
	CHECK(game.getGameProperty("openGL.hasGeometryShaders", var) == true);
	CHECK(game.getGameProperty("openGL.hasShaders", var) == true);
	CHECK(game.getGameProperty("openGL.majorVersion", var) == true);
	CHECK(game.getGameProperty("openGL.maxTextureSize", var) == true);
	CHECK(game.getGameProperty("openGL.minorVersion", var) == true);
	CHECK(game.getGameProperty("openGL.sRgbCapable", var) == true);
	CHECK(game.getGameProperty("openGL.stencilBits", var) == true);
	CHECK(game.getGameProperty("openGL.invalid", var) == false);

	CHECK(game.getGameProperty("path", var) == true);
	CHECK((std::holds_alternative<std::string>(var) && std::get<std::string>(var) == ""));

	CHECK(game.getGameProperty("position", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2()));

	CHECK(game.getGameProperty("position.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("position.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 0));

	CHECK(game.getGameProperty("refSize", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(640, 480)));

	CHECK(game.getGameProperty("refSize.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 640));

	CHECK(game.getGameProperty("refSize.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 480));

	FileUtils::setSaveDir(".");
	CHECK(game.getGameProperty("saveDir", var) == true);
	CHECK(std::holds_alternative<std::string>(var));

	CHECK(game.getGameProperty("scancodeKey.enter", var) == true);
	CHECK(std::holds_alternative<std::string>(var));

	CHECK(game.getGameProperty("size", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(640, 480)));

	CHECK(game.getGameProperty("size.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 640));

	CHECK(game.getGameProperty("size.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 480));

	CHECK(game.getGameProperty("smoothScreen", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("soundVolume", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 100));

	CHECK(game.getGameProperty("stretchToFit", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == false));

	CHECK(game.getGameProperty("title", var) == true);
	CHECK((std::holds_alternative<std::string>(var) && std::get<std::string>(var) == ""));

	CHECK(game.getGameProperty("version", var) == true);
	CHECK((std::holds_alternative<std::string>(var) && std::get<std::string>(var) == ""));

	CHECK(game.getGameProperty("windowedSize", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(640, 480)));

	CHECK(game.getGameProperty("windowedSize.x", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 640));

	CHECK(game.getGameProperty("windowedSize.y", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 480));

	CHECK(game.getGameProperty("invalid", var) == false);
}

TEST_CASE("Game::getProperty", "Game")
{
	Game game(true);
	auto obj = makeBitmapButton(game, "obj", makeTexture(game, "tex"));
	game.Resources().setCurrentLevel(obj);
	game.Resources().addFocused(obj);
	Variable var;

	CHECK(game.getProperty("$.2 + 2", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 4));

	CHECK(game.getProperty("eval.2 + 2", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 4));

	CHECK(game.getProperty("evalMin.2 + :rnd(10)", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 2));

	CHECK(game.getProperty("evalMax.2 + :rnd(10)", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 11));

	CHECK(game.getProperty("$f.2 + 2", var) == true);
	CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 4.0));

	CHECK(game.getProperty("evalf.2 + 2", var) == true);
	CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 4.0));

	CHECK(game.getProperty("evalMinf.2 + :rnd(10)", var) == true);
	CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 2.0));

	CHECK(game.getProperty("evalMaxf.2 + :rnd(10)", var) == true);
	CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 11.0));

	CHECK(game.getProperty("game.keepAR", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getProperty("currentLevel.enabled", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getProperty("focus.enabled", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getProperty("obj.enabled", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.getProperty("invalid", var) == false);
	CHECK(game.getProperty("game", var) == false);
	CHECK(game.getProperty("game.invalid", var) == false);
	CHECK(game.getProperty("currentLevel", var) == false);
	CHECK(game.getProperty("currentLevel.invalid", var) == false);
	CHECK(game.getProperty("obj", var) == false);
	CHECK(game.getProperty("obj.invalid", var) == false);
}

TEST_CASE("Game::ActionQueryable", "Game")
{
	Game game(true);
	auto action = std::make_shared<ActGameDraw>();

	CHECK(game.setAction(str2int16("close"), action) == true);
	CHECK(game.setAction(str2int16("focusGain"), action) == true);
	CHECK(game.setAction(str2int16("focusLoss"), action) == true);
	CHECK(game.setAction(str2int16("resize"), action) == true);
	CHECK(game.setAction(str2int16("invalid"), action) == false);

	CHECK(game.getAction(str2int16("close")) == action);
	CHECK(game.getAction(str2int16("focusGain")) == action);
	CHECK(game.getAction(str2int16("focusLoss")) == action);
	CHECK(game.getAction(str2int16("resize")) == action);
	CHECK(game.getAction(str2int16("CLOSE")) == nullptr);
	CHECK(game.getAction(str2int16("invalid")) == nullptr);
}

TEST_CASE("Game::getQueryAction", "Game")
{
	Game game(true);
	auto action = std::make_shared<ActGameDraw>();
	auto obj = makeBitmapButton(game, "obj", makeTexture(game, "tex"));

	CHECK(game.setAction(str2int16("close"), action) == true);
	CHECK(obj->setAction(str2int16("click"), action) == true);

	CHECK(game.getQueryAction("") == nullptr);
	CHECK(game.getQueryAction("invalid") == nullptr);
	CHECK(game.getQueryAction("game") == nullptr);
	CHECK(game.getQueryAction("game.close") == action);
	CHECK(game.getQueryAction("obj") == nullptr);
	CHECK(game.getQueryAction("obj.click") == action);
}

TEST_CASE("Game::getQueryable", "Game")
{
	Game game(true);
	auto obj = makeBitmapButton(game, "obj", makeTexture(game, "tex"));

	CHECK(getQueryablePtr(game.getQueryable("")) == &game);
	CHECK(getQueryablePtr(game.getQueryable("game")) == &game);
	CHECK(getQueryablePtr(game.getQueryable("obj")) == obj.get());
	CHECK(getQueryablePtr(game.getQueryable("currentLevel")) == nullptr);
	CHECK(getQueryablePtr(game.getQueryable("focus")) == nullptr);
	CHECK(getQueryablePtr(game.getQueryable("invalid")) == nullptr);

	game.Resources().setCurrentLevel(obj);
	game.Resources().addFocused(obj);

	CHECK(getQueryablePtr(game.getQueryable("currentLevel")) == obj.get());
	CHECK(getQueryablePtr(game.getQueryable("focus")) == obj.get());
}

TEST_CASE("Game::getQueryableList", "Game")
{
	Game game(true);

	auto vars = game.getQueryableList("");
	CHECK(vars.empty() == true);

	vars = game.getQueryableList("game.saveDirs");
	CHECK(vars.size() == 0);

	vars = game.getQueryableList("game.dirs");
	CHECK(vars.size() == 4);
	CHECK(std::get<std::string>(std::get<Variable>(vars.front())) == "dir1");
	CHECK(std::get<std::string>(std::get<Variable>(vars.back())) == "res");

	vars = game.getQueryableList("game.dirs.dir1");
	CHECK(vars.size() == 2);
	CHECK(std::get<std::string>(std::get<Variable>(vars.front())) == "subdir1");
	CHECK(std::get<std::string>(std::get<Variable>(vars.back())) == "subdir2");

	vars = game.getQueryableList("game.files.dir1");
	CHECK(vars.size() == 2);
	CHECK(std::get<std::string>(std::get<Variable>(vars.front())) == "file1.txt");
	CHECK(std::get<std::string>(std::get<Variable>(vars.back())) == "file2.json");
}
