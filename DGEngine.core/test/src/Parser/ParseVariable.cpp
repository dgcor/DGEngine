#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Game/Utils/VarUtils.h"
#include "Parser/ParseFile.h"

TEST_CASE("Parser::ParseVariable", "Parser")
{
	std::string json{ R"(
	{
	  "variable": [
	    { "int":          1234 },
	    { "double":       5.678 },
	    { "string":       "text!" },
	    { "bool":         true },
	    { "Int64Tuple2":  [-2, 3] },
	    { "Int64Tuple3":  [-2, 3, 1200] },
	    { "Int64Tuple4":  [-2, 3, 1200, -1298] },
	    { "DoubleTuple2": [-2.1, 3.9] },
	    { "DoubleTuple3": [-2.1, 3.9, 4.0] },
	    { "DoubleTuple4": [-2.1, 3.9, 4.0, 1200.87] },
	    { "MixedTuple2":  [-2.1, 10] },
	    { "MixedTuple3":  [10, -2.1, -2.5] },
	    { "MixedTuple4":  [10, -2.1, -2.5, 1] }
	  ]
	})"
	};

	Game game;
	Parser::parseJson(game, json);
	Variable var;

	CHECK(game.Variables().getVariableNoToken("invalid", var) == false);

	CHECK(game.Variables().getVariableNoToken("int", var) == true);
	CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 1234));

	CHECK(game.Variables().getVariableNoToken("double", var) == true);
	CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 5.678));

	CHECK(game.Variables().getVariableNoToken("string", var) == true);
	CHECK((std::holds_alternative<std::string>(var) && std::get<std::string>(var) == "text!"));

	CHECK(game.Variables().getVariableNoToken("bool", var) == true);
	CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

	CHECK(game.Variables().getVariableNoToken("Int64Tuple2", var) == true);
	CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(-2, 3)));

	CHECK(game.Variables().getVariableNoToken("Int64Tuple3", var) == true);
	CHECK((std::holds_alternative<Int64Tuple3>(var) && std::get<Int64Tuple3>(var) == Int64Tuple3(-2, 3, 1200)));

	CHECK(game.Variables().getVariableNoToken("Int64Tuple4", var) == true);
	CHECK((std::holds_alternative<Int64Tuple4>(var) && std::get<Int64Tuple4>(var) == Int64Tuple4(-2, 3, 1200, -1298)));

	CHECK(game.Variables().getVariableNoToken("DoubleTuple2", var) == true);
	CHECK((std::holds_alternative<DoubleTuple2>(var) && std::get<DoubleTuple2>(var) == DoubleTuple2(-2.1, 3.9)));

	CHECK(game.Variables().getVariableNoToken("DoubleTuple3", var) == true);
	CHECK((std::holds_alternative<DoubleTuple3>(var) && std::get<DoubleTuple3>(var) == DoubleTuple3(-2.1, 3.9, 4.0)));

	CHECK(game.Variables().getVariableNoToken("DoubleTuple4", var) == true);
	CHECK((std::holds_alternative<DoubleTuple4>(var) && std::get<DoubleTuple4>(var) == DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)));

	CHECK(game.Variables().getVariableNoToken("MixedTuple2", var) == false);
	CHECK(game.Variables().getVariableNoToken("MixedTuple3", var) == false);
	CHECK(game.Variables().getVariableNoToken("MixedTuple4", var) == false);
}
