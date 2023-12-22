#include "catch2/catch_test_macros.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "Json/JsonUtils.h"
#include "Parser/Utils/ParseUtils.h"

TEST_CASE("Parser::getReplaceVars", "Parser")
{
	CHECK(Parser::getReplaceVars("none") == ReplaceVars::None);
	CHECK(Parser::getReplaceVars("string") == ReplaceVars::String);
	CHECK(Parser::getReplaceVars("value") == ReplaceVars::Value);
	CHECK(Parser::getReplaceVars("VALUE") == ReplaceVars::Value);
	CHECK(Parser::getReplaceVars("value ") == ReplaceVars::None);
	CHECK(Parser::getReplaceVars("invalid", ReplaceVars::Value) == ReplaceVars::Value);
	CHECK(Parser::getReplaceVars("") == ReplaceVars::None);
}

TEST_CASE("Parser::getIdFromFile", "Parser")
{
	std::string id("text");

	CHECK(Parser::getIdFromFile("", id) == false);
	CHECK(id == "");

	CHECK(Parser::getIdFromFile("file", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("file.txt", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("path 1/file.txt", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("path 1/file/...fi.le.txt", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("path 1//file.", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("path 1//file..", id) == true);
	CHECK(id == "file");

	CHECK(Parser::getIdFromFile("path 1//.txt", id) == true);
	CHECK(id == "txt");

	CHECK(Parser::getIdFromFile("path 1//file/", id) == false);
	CHECK(id == "");

	CHECK(Parser::getIdFromFile("path 1/file/.", id) == false);
	CHECK(id == "");

	CHECK(Parser::getIdFromFile("path 1/file/..", id) == false);
	CHECK(id == "");

	CHECK(Parser::getIdFromFile("path 1/file/...", id) == false);
	CHECK(id == "");
}

TEST_CASE("Parser::isValidArray", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": null,
	  "key03": 1,
	  "key04": true,
	  "key05": [],
	  "key06": ["text"],
	  "key07": ["text", 1],
	  "key08": [1.2, null],
	  "key09": [[10, 20], [true, false]]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::isValidArray(doc, "key01") == false);
	CHECK(Parser::isValidArray(doc, "key02") == false);
	CHECK(Parser::isValidArray(doc, "key03") == false);
	CHECK(Parser::isValidArray(doc, "key04") == false);
	CHECK(Parser::isValidArray(doc, "key05") == false);
	CHECK(Parser::isValidArray(doc, "key06") == true);
	CHECK(Parser::isValidArray(doc, "key07") == true);
	CHECK(Parser::isValidArray(doc, "key08") == true);
	CHECK(Parser::isValidArray(doc, "key09") == true);
	CHECK(Parser::isValidArray(doc, "invalid") == false);
	CHECK(Parser::isValidArray(doc["key01"], "invalid") == false);
}

TEST_CASE("Parser::isValidId", "Parser")
{
	CHECK(Parser::isValidId("") == false);
	CHECK(Parser::isValidId("_") == true);
	CHECK(Parser::isValidId("abcdefghijklmnopqrstuvwxyz") == true);
	CHECK(Parser::isValidId("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == true);
	CHECK(Parser::isValidId("0123456789_") == true);
	CHECK(Parser::isValidId("àéîöu") == false);
	CHECK(Parser::isValidId("ab cd") == false);
	CHECK(Parser::isValidId("12 24") == false);
	CHECK(Parser::isValidId("ab ") == false);
	CHECK(Parser::isValidId(" ab") == false);
	CHECK(Parser::isValidId("ab.cd") == false);
	CHECK(Parser::isValidId("12.24") == false);
	CHECK(Parser::isValidId("ab.") == false);
	CHECK(Parser::isValidId(".ab") == false);
}

TEST_CASE("Parser::isValidObject", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": null,
	  "key03": 1,
	  "key04": true,
	  "key05": {},
	  "key06": { "key": 1 },
	  "key07": { "key": 1, "key2": "text" }
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::isValidObject(doc, "key01") == false);
	CHECK(Parser::isValidObject(doc, "key02") == false);
	CHECK(Parser::isValidObject(doc, "key03") == false);
	CHECK(Parser::isValidObject(doc, "key04") == false);
	CHECK(Parser::isValidObject(doc, "key05") == false);
	CHECK(Parser::isValidObject(doc, "key06") == true);
	CHECK(Parser::isValidObject(doc, "key07") == true);
	CHECK(Parser::isValidObject(doc, "invalid") == false);
	CHECK(Parser::isValidObject(doc["key01"], "invalid") == false);
}

TEST_CASE("Parser::isValidString", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": "",
	  "key03": null,
	  "key04": 1,
	  "key05": true,
	  "key06": ["text"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("value")
	{
		CHECK(Parser::isValidString(doc["key01"]) == true);
		CHECK(Parser::isValidString(doc["key02"]) == false);
	}
	SECTION("key")
	{
		CHECK(Parser::isValidString(doc, "key01") == true);
		CHECK(Parser::isValidString(doc, "key02") == false);
		CHECK(Parser::isValidString(doc, "key03") == false);
		CHECK(Parser::isValidString(doc, "key04") == false);
		CHECK(Parser::isValidString(doc, "key05") == false);
		CHECK(Parser::isValidString(doc, "key06") == false);
		CHECK(Parser::isValidString(doc, "invalid") == false);
		CHECK(Parser::isValidString(doc["key01"], "invalid") == false);
	}
}
