#include "catch2/catch_test_macros.hpp"

#include "Json/JsonUtils.h"
#include "Parser/ParseCommon.h"

TEST_CASE("Parser::parseValidId", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "id": "id1", "key": 10 },
	  "key02": { "id": "id 1", "key": 10 },
	  "key03": { "id": 5, "key": 10 },
	  "key04": { "key": "id1" },
	  "key05": "id"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::parseValidId(doc["key01"]) == "id1");
	CHECK(Parser::parseValidId(doc["key02"]) == "");
	CHECK(Parser::parseValidId(doc["key03"]) == "");
	CHECK(Parser::parseValidId(doc["key04"]) == "");
	CHECK(Parser::parseValidId(doc["key05"]) == "");
}

TEST_CASE("Parser::parseValidIdOrFile", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "id": "id1", "file": "path/file1.txt" },
	  "key02": { "id": "id 1", "file": "path/file1.txt" },
	  "key03": { "is": "id 1", "file": "path/file1.txt" },
	  "key04": { "is": "id 1", "file": "file1" },
	  "key05": { "is": "id 1", "file": ".txt" },
	  "key06": { "is": "id 1", "fila": "file1.txt" },
	  "key07": { "is": 10, "fila": "file1.txt" },
	  "key08": "id"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::parseValidIdOrFile(doc["key01"]) == "id1");
	CHECK(Parser::parseValidIdOrFile(doc["key02"]) == "");
	CHECK(Parser::parseValidIdOrFile(doc["key03"]) == "file1");
	CHECK(Parser::parseValidIdOrFile(doc["key04"]) == "file1");
	CHECK(Parser::parseValidIdOrFile(doc["key05"]) == "txt");
	CHECK(Parser::parseValidIdOrFile(doc["key06"]) == "");
	CHECK(Parser::parseValidIdOrFile(doc["key06"], "fila") == "file1");
	CHECK(Parser::parseValidIdOrFile(doc["key07"]) == "");
	CHECK(Parser::parseValidIdOrFile(doc["key08"]) == "");
}

TEST_CASE("Parser::parseValidIdOrFilePath", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "id": "id1", "file": "path/file1.txt" },
	  "key02": { "id": "id 1", "file": "path/file1.txt" },
	  "key03": { "is": "id 1", "file": "path/file1.txt" },
	  "key04": { "is": 10, "fila": "file1.txt" },
	  "key05": "id"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::parseValidIdOrFilePath(doc["key01"], "file2.txt") == "id1");
	CHECK(Parser::parseValidIdOrFilePath(doc["key02"], "file2.txt") == "");
	CHECK(Parser::parseValidIdOrFilePath(doc["key03"], "file2.txt") == "file2");
	CHECK(Parser::parseValidIdOrFilePath(doc["key04"], ".txt") == "txt");
	CHECK(Parser::parseValidIdOrFilePath(doc["key05"], "path/a.txt") == "a");
	CHECK(Parser::parseValidIdOrFilePath(doc["key05"], " ") == "");
}
