#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/Circle.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/VarUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/Utils/ParseUtilsKey.h"

TEST_CASE("JsonUtils::query", "JsonUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "document": {
	    "key01": {
	      "subKey01": 1234
	    },
	    "key02": [
	      {
	        "name": "first",
	        "subKey01": 1234
	      },
	      {
	        "name": "second",
	        "subKey01": 5678
	      }
	    ]
	  },
	  "query01": "/key01/subKey01",
	  "query02": "/key02",
	  "query03": "/key02/{name=second}/subKey01",
	  "query04": "/key02/1",
	  "query05": "/key02/1/subKey01",
	  "query06": "/key01/subKey01/",
	  "query07": "/key02/{name=first}",
	  "query08": "/key02/{name=first}/",
	  "query09": "/key02/1/",
	  "query10": "/key02/2/subKey01",
	  "query11": "/key02//",
	  "query12": "/key02/{name=first/subKey02",
	  "query13": "/key02/{{name=first}/subKey02",
	  "query14": "/key02/{{name=first}}/subKey02",
	  "query15": "/key03/0/subKey01",
	  "query16": "/key03/{name=first}/subKey02",
	  "query17": "invalid",
	  "query18": "",
	  "query19": true,
	  "query20": false,
	  "query21": ["value"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("valid")
	{
		CHECK(JsonUtils::query(doc["document"], doc["query01"]) == doc["document"]["key01"]["subKey01"]);
		CHECK(JsonUtils::query(doc["document"], doc["query02"]) == doc["document"]["key02"]);
		CHECK(JsonUtils::query(doc["document"], doc["query03"]) == doc["document"]["key02"][1]["subKey01"]);
		CHECK(JsonUtils::query(doc["document"], doc["query04"]) == doc["document"]["key02"][1]);
		CHECK(JsonUtils::query(doc["document"], doc["query05"]) == doc["document"]["key02"][1]["subKey01"]);
	}
	SECTION("invalid")
	{
		CHECK(JsonUtils::query(doc["document"], doc["query06"]) == doc["query06"]);
		CHECK(JsonUtils::query(doc["document"], doc["query07"]) == doc["query07"]);
		CHECK(JsonUtils::query(doc["document"], doc["query08"]) == doc["query08"]);
		CHECK(JsonUtils::query(doc["document"], doc["query09"]) == doc["query09"]);
		CHECK(JsonUtils::query(doc["document"], doc["query10"]) == doc["query10"]);
		CHECK(JsonUtils::query(doc["document"], doc["query11"]) == doc["query11"]);
		CHECK(JsonUtils::query(doc["document"], doc["query12"]) == doc["query12"]);
		CHECK(JsonUtils::query(doc["document"], doc["query13"]) == doc["query13"]);
		CHECK(JsonUtils::query(doc["document"], doc["query14"]) == doc["query14"]);
		CHECK(JsonUtils::query(doc["document"], doc["query15"]) == doc["query15"]);
		CHECK(JsonUtils::query(doc["document"], doc["query16"]) == doc["query16"]);
		CHECK(JsonUtils::query(doc["document"], doc["query17"]) == doc["query17"]);
		CHECK(JsonUtils::query(doc["document"], doc["query18"]) == doc["query18"]);
		CHECK(JsonUtils::query(doc["document"], doc["query19"]) == doc["query19"]);
		CHECK(JsonUtils::query(doc["document"], doc["query20"]) == doc["query20"]);
		CHECK(JsonUtils::query(doc["document"], doc["query21"]) == doc["query21"]);
	}
}

TEST_CASE("JsonUtils::replaceStringValueWithQueryable", "GameUtils")
{
	Circle obj(2.f, 30);
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "circle radius: %radius%",
	  "key02": "circle radius: %radius%",
	  "key03": 12,
	  "key04": "%radius%",
	  "key05": "%radius%",
	  "key06": "%radius% ",
	  "key07": "%radius% ",
	  "key08": { "subKey1": "%radius%" },
	  "key09": "%%radius%",
	  "key10": "%radius%%",
	  "key11": "%%radius%%"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	JsonUtils::replaceStringValueWithQueryable(doc["key01"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key02"], doc.GetAllocator(), obj, true);
	JsonUtils::replaceStringValueWithQueryable(doc["key03"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key04"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key05"], doc.GetAllocator(), obj, true);
	JsonUtils::replaceStringValueWithQueryable(doc["key06"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key07"], doc.GetAllocator(), obj, true);
	JsonUtils::replaceStringValueWithQueryable(doc["key08"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key09"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key10"], doc.GetAllocator(), obj);
	JsonUtils::replaceStringValueWithQueryable(doc["key11"], doc.GetAllocator(), obj);

	CHECK(Parser::getStringViewKey(doc, "key01") == "circle radius: 2");
	CHECK(Parser::getStringViewKey(doc, "key02") == "circle radius: 2");
	CHECK(Parser::getIntKey(doc, "key03") == 12);
	CHECK(Parser::getStringViewKey(doc, "key04") == "2");
	CHECK(Parser::getIntKey(doc, "key05") == 2);
	CHECK(Parser::getStringViewKey(doc, "key06") == "2 ");
	CHECK(Parser::getStringViewKey(doc, "key07") == "2 ");
	CHECK(Parser::getStringViewKey(doc["key08"], "subKey1") == "%radius%");
	CHECK(Parser::getStringViewKey(doc, "key09") == "%%radius%");
	CHECK(Parser::getStringViewKey(doc, "key10") == "2%");
	CHECK(Parser::getStringViewKey(doc, "key11") == "%%radius%%");
}

TEST_CASE("JsonUtils::replaceStringValueWithVariable", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 12,
	  "key02": "text", "key03": "text", "key04": "text", "key05": "text",
	  "key06": "text", "key07": "text", "key08": "text", "key09": "text",
	  "key10": "text", "key11": "text", "key12": "text", "key13": "text",
	  "key14": "text", "key15": "text", "key16": "text", "key17": "text",
	  "key18": "text", "key19": "text", "key20": "text", "key21": "text",
	  "key22": { "subKey01": "text" }
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	JsonUtils::replaceStringValueWithVariable(doc["key01"], doc.GetAllocator(), "text", "game");
	JsonUtils::replaceStringValueWithVariable(doc["key02"], doc.GetAllocator(), "text", "game");
	JsonUtils::replaceStringValueWithVariable(doc["key03"], doc.GetAllocator(), "text", "game", true);
	JsonUtils::replaceStringValueWithVariable(doc["key04"], doc.GetAllocator(), "text", 15);
	JsonUtils::replaceStringValueWithVariable(doc["key05"], doc.GetAllocator(), "text", 15, true);
	JsonUtils::replaceStringValueWithVariable(doc["key06"], doc.GetAllocator(), "text", 1.5);
	JsonUtils::replaceStringValueWithVariable(doc["key07"], doc.GetAllocator(), "text", 1.5, true);
	JsonUtils::replaceStringValueWithVariable(doc["key08"], doc.GetAllocator(), "text", true);
	JsonUtils::replaceStringValueWithVariable(doc["key09"], doc.GetAllocator(), "text", true, true);
	JsonUtils::replaceStringValueWithVariable(doc["key10"], doc.GetAllocator(), "text", Int64Tuple2(1, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key11"], doc.GetAllocator(), "text", Int64Tuple2(1, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key12"], doc.GetAllocator(), "text", Int64Tuple3(1, 1, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key13"], doc.GetAllocator(), "text", Int64Tuple3(1, 1, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key14"], doc.GetAllocator(), "text", Int64Tuple4(1, 1, 1, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key15"], doc.GetAllocator(), "text", Int64Tuple4(1, 1, 1, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key16"], doc.GetAllocator(), "text", DoubleTuple2(1.5, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key17"], doc.GetAllocator(), "text", DoubleTuple2(1.5, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key18"], doc.GetAllocator(), "text", DoubleTuple3(1.5, 1, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key19"], doc.GetAllocator(), "text", DoubleTuple3(1.5, 1, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key20"], doc.GetAllocator(), "text", DoubleTuple4(1.5, 1, 1, 1));
	JsonUtils::replaceStringValueWithVariable(doc["key21"], doc.GetAllocator(), "text", DoubleTuple4(1.5, 1, 1, 1), true);
	JsonUtils::replaceStringValueWithVariable(doc["key22"], doc.GetAllocator(), "text", "game");

	CHECK(Parser::getIntKey(doc, "key01") == 12);
	CHECK(Parser::getStringViewKey(doc, "key02") == "game");
	CHECK(Parser::getStringViewKey(doc, "key03") == "game");
	CHECK(Parser::getStringViewKey(doc, "key04") == "15");
	CHECK(Parser::getIntKey(doc, "key05") == 15);
	CHECK(Parser::getStringViewKey(doc, "key06") == "1.5");
	CHECK(Parser::getDoubleKey(doc, "key07") == 1.5);
	CHECK(Parser::getStringViewKey(doc, "key08") == "true");
	CHECK(Parser::getBoolKey(doc, "key09") == true);
	CHECK(Parser::getStringViewKey(doc, "key10") == "[1, 1]");
	CHECK(Parser::getVector2iKey<Int64Tuple2>(doc, "key11") == Int64Tuple2(1, 1));
	CHECK(Parser::getStringViewKey(doc, "key12") == "[1, 1, 1]");
	CHECK(Parser::getVector3iKey<Int64Tuple3>(doc, "key13") == Int64Tuple3(1, 1, 1));
	CHECK(Parser::getStringViewKey(doc, "key14") == "[1, 1, 1, 1]");
	CHECK(Parser::getVector4iKey<Int64Tuple4>(doc, "key15") == Int64Tuple4(1, 1, 1, 1));
	CHECK(Parser::getStringViewKey(doc, "key16") == "[1.5, 1.0]");
	CHECK(Parser::getVector2fKey<DoubleTuple2>(doc, "key17") == DoubleTuple2(1.5, 1));
	CHECK(Parser::getStringViewKey(doc, "key18") == "[1.5, 1.0, 1.0]");
	CHECK(Parser::getVector3fKey<DoubleTuple3>(doc, "key19") == DoubleTuple3(1.5, 1, 1));
	CHECK(Parser::getStringViewKey(doc, "key20") == "[1.5, 1.0, 1.0, 1.0]");
	CHECK(Parser::getVector4fKey<DoubleTuple4>(doc, "key21") == DoubleTuple4(1.5, 1, 1, 1));
	CHECK(Parser::getStringViewKey(doc["key22"], "subKey01") == "text");
}

TEST_CASE("JsonUtils::replaceValueWithVariable", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("string")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), "text");
		CHECK(Parser::getStringKey(doc, "key01") == "text");
	}
	SECTION("int64_t")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), 1200);
		CHECK(Parser::getIntKey(doc, "key01") == 1200);
	}
	SECTION("double")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), 1.5);
		CHECK(Parser::getDoubleKey(doc, "key01") == 1.5);
	}
	SECTION("bool")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), true);
		CHECK(Parser::getBoolKey(doc, "key01") == true);
	}
	SECTION("Int64Tuple2")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), Int64Tuple2(1, 1));
		CHECK(Parser::getVector2iKey<sf::Vector2i>(doc, "key01") == sf::Vector2i(1, 1));
	}
	SECTION("Int64Tuple3")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), Int64Tuple3(1, 1, 1));
		CHECK(Parser::getVector3iKey<sf::Vector3i>(doc, "key01") == sf::Vector3i(1, 1, 1));
	}
	SECTION("Int64Tuple4")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), Int64Tuple4(1, 1, 1, 1));
		CHECK(Parser::getVector4iKey<Int64Tuple4>(doc, "key01") == Int64Tuple4(1, 1, 1, 1));
	}
	SECTION("DoubleTuple2")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), DoubleTuple2(1.5, 1));
		CHECK(Parser::getVector2fKey<sf::Vector2f>(doc, "key01") == sf::Vector2f(1.5, 1));
	}
	SECTION("DoubleTuple3")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), DoubleTuple3(1.5, 1, 1));
		CHECK(Parser::getVector3fKey<sf::Vector3f>(doc, "key01") == sf::Vector3f(1.5, 1, 1));
	}
	SECTION("DoubleTuple4")
	{
		JsonUtils::replaceValueWithVariable(doc["key01"], doc.GetAllocator(), DoubleTuple4(1.5, 1, 1, 1));
		CHECK(Parser::getVector4fKey<DoubleTuple4>(doc, "key01") == DoubleTuple4(1.5, 1, 1, 1));
	}
}

TEST_CASE("JsonUtils::replaceValuesWithFunction", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text1",
	  "key02": 12,
	  "key03": "text2"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	auto allocator = doc.GetAllocator();

	JsonUtils::replaceValuesWithFunction(doc, allocator,
		[&allocator](rapidjson::Value& val)
		{
			if (val.IsString() && val.GetStringView().ends_with("1"))
			{
				val.SetString("game", allocator);
			}
			else if (val.IsInt())
			{
				val.SetDouble(2.5);
			}
		});

	CHECK(Parser::getStringViewKey(doc, "key01") == "game");
	CHECK(Parser::getIntKey(doc, "key02") == 12);
	CHECK(Parser::getStringViewKey(doc, "key03") == "text2");
}

TEST_CASE("JsonUtils::replaceValuesWithString", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "This text is good Text",
	  "key02": 12,
	  "key03": "text"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	JsonUtils::replaceValuesWithString(doc, doc.GetAllocator(), "text", "game");

	CHECK(Parser::getStringViewKey(doc, "key01") == "This game is good Text");
	CHECK(Parser::getIntKey(doc, "key02") == 12);
	CHECK(Parser::getStringViewKey(doc, "key03") == "game");
}

TEST_CASE("JsonUtils::replaceValuesWithGameVar", "GameUtils")
{
	Game game(true);
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": "game.size.x",
	  "key03": "%game.size.x%",
	  "key04": "!game.size.x!",
	  "key05": "game size: [%game.size.x%, %game.size.y%]",
	  "key06": "%game.keepAR%",
	  "key07": "%game.size%",
	  "key08": "%game.keepAR% ",
	  "key09": "game size: [%%game.size.x%, %game.size.y%]",
	  "key10": "game size: [%%game.size.x%, %game.size.y%%]",
	  "key11": "game size: [%%game.size.x%%, %game.size.y%%]",
	  "key12": "game size: [%%game.size.x%,% %game.size.y%%]"
	})"
	};

	SECTION("no change value type")
	{
		CHECK(JsonUtils::loadJson(json, doc) == true);

		JsonUtils::replaceValuesWithGameVar(doc, doc.GetAllocator(), game, false);

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getStringViewKey(doc, "key01") == "text");
			CHECK(Parser::getStringViewKey(doc, "key02") == "game.size.x");
			CHECK(Parser::getStringViewKey(doc, "key03") == "640");
			CHECK(Parser::getStringViewKey(doc, "key04") == "!game.size.x!");
			CHECK(Parser::getStringViewKey(doc, "key05") == "game size: [640, 480]");
			CHECK(Parser::getStringViewKey(doc, "key06") == "true");
			CHECK(Parser::getStringViewKey(doc, "key07") == "[640, 480]");
			CHECK(Parser::getStringViewKey(doc, "key08") == "true ");
			CHECK(Parser::getStringViewKey(doc, "key09") == "game size: [%640, 480]");
			CHECK(Parser::getStringViewKey(doc, "key10") == "game size: [%640, 480%]");
			CHECK(Parser::getStringViewKey(doc, "key11") == "game size: [%640%, 480%]");
			CHECK(Parser::getStringViewKey(doc, "key12") == "game size: [%640,% 480%]");
		}
	}
	SECTION("change value type")
	{
		CHECK(JsonUtils::loadJson(json, doc) == true);

		JsonUtils::replaceValuesWithGameVar(doc, doc.GetAllocator(), game, true);

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getStringViewKey(doc, "key01") == "text");
			CHECK(Parser::getStringViewKey(doc, "key02") == "game.size.x");
			CHECK(Parser::getIntKey(doc, "key03") == 640);
			CHECK(Parser::getStringViewKey(doc, "key04") == "!game.size.x!");
			CHECK(Parser::getStringViewKey(doc, "key05") == "game size: [640, 480]");
			CHECK(Parser::getBoolKey(doc, "key06") == true);
			CHECK(Parser::getVector2iKey<sf::Vector2i>(doc, "key07") == sf::Vector2i(640, 480));
			CHECK(Parser::getBoolKey(doc, "key08") == false);
		}
	}
	SECTION("different token")
	{
		CHECK(JsonUtils::loadJson(json, doc) == true);

		JsonUtils::replaceValuesWithGameVar(doc, doc.GetAllocator(), game, true, '!');

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getStringViewKey(doc, "key03") == "%game.size.x%");
			CHECK(Parser::getIntKey(doc, "key04") == 640);
			CHECK(Parser::getStringViewKey(doc, "key05") == "game size: [%game.size.x%, %game.size.y%]");
		}
	}
}

TEST_CASE("JsonUtils::toString", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": -1,
	  "key03": 5.89,
	  "key04": true,
	  "key05": false,
	  "key06": null,
	  "key07": ["value", "string"],
	  "key08": { "name": "object", "range": [1, 10], "unique": true }
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(JsonUtils::toString(doc["key01"]) == "text");
	CHECK(JsonUtils::toString(doc["key02"]) == "-1");
	CHECK(JsonUtils::toString(doc["key03"]) == "5.89");
	CHECK(JsonUtils::toString(doc["key04"]) == "true");
	CHECK(JsonUtils::toString(doc["key05"]) == "false");
	CHECK(JsonUtils::toString(doc["key06"]) == "null");
	CHECK(JsonUtils::toString(doc["key07"]) == R"(["value","string"])");
	CHECK(JsonUtils::toString(doc["key08"]) == R"({"name":"object","range":[1,10],"unique":true})");
}

TEST_CASE("JsonUtils::jsonToString", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": -1,
	  "key03": 5.89,
	  "key04": true,
	  "key05": false,
	  "key06": null,
	  "key07": ["value", "string"],
	  "key08": { "name": "object", "range": [1, 10], "unique": true }
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(JsonUtils::jsonToString(doc["key01"]) == R"("text")");
	CHECK(JsonUtils::jsonToString(doc["key02"]) == "-1");
	CHECK(JsonUtils::jsonToString(doc["key03"]) == "5.89");
	CHECK(JsonUtils::jsonToString(doc["key04"]) == "true");
	CHECK(JsonUtils::jsonToString(doc["key05"]) == "false");
	CHECK(JsonUtils::jsonToString(doc["key06"]) == "null");
	CHECK(JsonUtils::jsonToString(doc["key07"]) == R"(["value","string"])");
	CHECK(JsonUtils::jsonToString(doc["key08"]) == R"({"name":"object","range":[1,10],"unique":true})");
}

TEST_CASE("JsonUtils::jsonToPrettyString", "GameUtils")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": -1,
	  "key03": 5.89,
	  "key04": true,
	  "key05": false,
	  "key06": null,
	  "key07": ["value", "string"],
	  "key08": { "name": "object", "range": [1, 10], "unique": true }
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(JsonUtils::jsonToPrettyString(doc["key01"]) == R"("text")");
	CHECK(JsonUtils::jsonToPrettyString(doc["key02"]) == "-1");
	CHECK(JsonUtils::jsonToPrettyString(doc["key03"]) == "5.89");
	CHECK(JsonUtils::jsonToPrettyString(doc["key04"]) == "true");
	CHECK(JsonUtils::jsonToPrettyString(doc["key05"]) == "false");
	CHECK(JsonUtils::jsonToPrettyString(doc["key06"]) == "null");
	CHECK(JsonUtils::jsonToPrettyString(doc["key07"]) == R"(["value", "string"])");
	CHECK(JsonUtils::jsonToPrettyString(doc["key08"]) == "{\n  \"name\": \"object\",\n  \"range\": [1, 10],\n  \"unique\": true\n}");
}

TEST_CASE("JsonUtils::loadFile", "GameUtils")
{
	rapidjson::Document doc;

	SECTION("valid")
	{
		CHECK(JsonUtils::loadFile("json/jsonFile.json", doc) == true);

		if (doc.IsNull() == false &&
			doc.IsObject() == true &&
			doc.MemberCount() == 8)
		{
			CHECK(JsonUtils::jsonToString(doc["key01"]) == R"("text")");
			CHECK(JsonUtils::jsonToString(doc["key02"]) == "-1");
			CHECK(JsonUtils::jsonToString(doc["key03"]) == "5.89");
			CHECK(JsonUtils::jsonToString(doc["key04"]) == "true");
			CHECK(JsonUtils::jsonToString(doc["key05"]) == "false");
			CHECK(JsonUtils::jsonToString(doc["key06"]) == "null");
			CHECK(JsonUtils::jsonToString(doc["key07"]) == R"(["value","string"])");
			CHECK(JsonUtils::jsonToString(doc["key08"]) == R"({"name":"object","range":[1,10],"unique":true})");
		}
	}
	SECTION("invalid")
	{
		CHECK(JsonUtils::loadFile("resources/invalidFile.json", doc) == false);
		CHECK(doc.IsNull() == true);
	}
}

TEST_CASE("JsonUtils::loadJsonAndReplaceValues", "GameUtils")
{
	Game game(true);
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "%game.size.x%",
	  "key02": "!game.size.x!"
	})"
	};

	SECTION("no change value type")
	{
		CHECK(JsonUtils::loadJsonAndReplaceValues(json, doc, game, false) == true);

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getStringViewKey(doc, "key01") == "640");
			CHECK(Parser::getStringViewKey(doc, "key02") == "!game.size.x!");
		}
	}
	SECTION("change value type")
	{
		CHECK(JsonUtils::loadJsonAndReplaceValues(json, doc, game, true) == true);

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getIntKey(doc, "key01") == 640);
			CHECK(Parser::getStringViewKey(doc, "key02") == "!game.size.x!");
		}
	}
	SECTION("different token")
	{
		CHECK(JsonUtils::loadJsonAndReplaceValues(json, doc, game, true, '!') == true);

		CHECK(doc.IsNull() == false);
		if (doc.IsNull() == false)
		{
			CHECK(doc.IsObject() == true);
			CHECK(Parser::getStringViewKey(doc, "key01") == "%game.size.x%");
			CHECK(Parser::getIntKey(doc, "key02") == 640);
		}
	}
}

TEST_CASE("JsonUtils::saveToFile", "GameUtils")
{
	std::string json{ R"(
	{
	  "variable": [
	    { "key01": 1234 },
	    { "key02": 5.678 },
	    { "key03": "text!" },
	    { "key04": true },
	    { "key05": [-2, 3] },
	    { "key06": [-2, 3, 1200] },
	    { "key07": [-2, 3, 1200, -1298] },
	    { "key08": [-2.1, 3.9] },
	    { "key09": [-2.1, 3.9, 4.0] },
	    { "key10": [-2.1, 3.9, 4.0, 1200.87] },
	    { "key11": [-2.1, 10] },
	    { "key12": [10, -2.1, -2.5] },
	    { "key13": [10, -2.1, -2.5, 1] }
	  ]
	}
	)" };

	SECTION("initialize")
	{
		CHECK(FileUtils::setSaveDir("dir1") == true);
		CHECK(FileUtils::mount(FileUtils::getSaveDir(), "", true) == true);
	}
	SECTION("json")
	{
		rapidjson::Document doc;
		rapidjson::Document doc2;

		CHECK(JsonUtils::loadJson(json, doc) == true);
		JsonUtils::saveToFile("saveFile1.json", doc);
		CHECK(JsonUtils::loadFile("saveFile1.json", doc2) == true);

		CHECK(Parser::getStringKey(doc["variable"], "key01") == Parser::getStringKey(doc2["variable"], "key01"));
		CHECK(Parser::getStringKey(doc["variable"], "key02") == Parser::getStringKey(doc2["variable"], "key02"));
		CHECK(Parser::getStringKey(doc["variable"], "key03") == Parser::getStringKey(doc2["variable"], "key03"));
		CHECK(Parser::getStringKey(doc["variable"], "key04") == Parser::getStringKey(doc2["variable"], "key04"));
		CHECK(Parser::getStringKey(doc["variable"], "key05") == Parser::getStringKey(doc2["variable"], "key05"));
		CHECK(Parser::getStringKey(doc["variable"], "key06") == Parser::getStringKey(doc2["variable"], "key06"));
		CHECK(Parser::getStringKey(doc["variable"], "key07") == Parser::getStringKey(doc2["variable"], "key07"));
		CHECK(Parser::getStringKey(doc["variable"], "key08") == Parser::getStringKey(doc2["variable"], "key08"));
		CHECK(Parser::getStringKey(doc["variable"], "key09") == Parser::getStringKey(doc2["variable"], "key09"));
		CHECK(Parser::getStringKey(doc["variable"], "key10") == Parser::getStringKey(doc2["variable"], "key10"));
		CHECK(Parser::getStringKey(doc["variable"], "key11") == Parser::getStringKey(doc2["variable"], "key11"));
		CHECK(Parser::getStringKey(doc["variable"], "key12") == Parser::getStringKey(doc2["variable"], "key12"));
		CHECK(Parser::getStringKey(doc["variable"], "key13") == Parser::getStringKey(doc2["variable"], "key13"));
	}
	SECTION("vector")
	{
		rapidjson::Document doc;

		std::vector<std::pair<std::string, Variable>> variables =
		{
			{ std::string("key01"), Variable(1234) },
			{ std::string("key02"), Variable(5.678) },
			{ std::string("key03"), Variable("text!") },
			{ std::string("key04"), Variable(true) },
			{ std::string("key05"), Variable(Int64Tuple2(-2, 3)) },
			{ std::string("key06"), Variable(Int64Tuple3(-2, 3, 1200)) },
			{ std::string("key07"), Variable(Int64Tuple4(-2, 3, 1200, -1298)) },
			{ std::string("key08"), Variable(DoubleTuple2(-2.1, 3.9)) },
			{ std::string("key09"), Variable(DoubleTuple3(-2.1, 3.9, 4.0)) },
			{ std::string("key10"), Variable(DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)) }
		};

		JsonUtils::saveToFile("saveFile2.json", "variable", variables);
		CHECK(JsonUtils::loadFile("saveFile2.json", doc) == true);

		CHECK(VarUtils::toString(variables[0].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key01"]));
		CHECK(VarUtils::toString(variables[1].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key02"]));
		CHECK(VarUtils::toString(variables[2].second) == JsonUtils::toString(doc["variable"]["key03"]));
		CHECK(VarUtils::toString(variables[3].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key04"]));
		CHECK(VarUtils::toString(variables[4].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key05"]));
		CHECK(VarUtils::toString(variables[5].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key06"]));
		CHECK(VarUtils::toString(variables[6].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key07"]));
		CHECK(VarUtils::toString(variables[7].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key08"]));
		CHECK(VarUtils::toString(variables[8].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key09"]));
		CHECK(VarUtils::toString(variables[9].second) == JsonUtils::jsonToPrettyString(doc["variable"]["key10"]));
	}
	SECTION("unmount save dir")
	{
		CHECK(FileUtils::deleteFile("saveFile1.json") == true);
		CHECK(FileUtils::deleteFile("saveFile2.json") == true);
		CHECK(FileUtils::unmount(FileUtils::getSaveDir()) == true);
	}
}
