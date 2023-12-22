#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Json/JsonUtils.h"
#include "Parser/Utils/ParseUtilsKey.h"

TEST_CASE("Parser::getAnchorKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "all" },
	  "key02": "all",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getAnchorKey(doc["key01"], "key01") == Anchor::All);
	CHECK(Parser::getAnchorKey(doc["key02"], "key02", Anchor::Left) == Anchor::Left);
	CHECK(Parser::getAnchorKey(doc["key03"], "key03") == (Anchor::Top | Anchor::Left));
}

TEST_CASE("Parser::getAnimationTypeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "playOnce" },
	  "key02": "playOnce",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getAnimationTypeKey(doc["key01"], "key01") == AnimationType::PlayOnce);
	CHECK(Parser::getAnimationTypeKey(doc["key02"], "key02", AnimationType::BackAndForth) == AnimationType::BackAndForth);
	CHECK(Parser::getAnimationTypeKey(doc["key03"], "key03") == AnimationType::Looped);
}

TEST_CASE("Parser::getBindingFlagsKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "always" },
	  "key02": "always",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBindingFlagsKey(doc["key01"], "key01") == BindingFlags::Always);
	CHECK(Parser::getBindingFlagsKey(doc["key02"], "key02", BindingFlags::Once) == BindingFlags::Once);
	CHECK(Parser::getBindingFlagsKey(doc["key03"], "key03") == BindingFlags::OnChange);
}

TEST_CASE("Parser::getBlendModeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "add" },
	  "key02": "add",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBlendModeKey(doc["key01"], "key01") == BlendMode::Add);
	CHECK(Parser::getBlendModeKey(doc["key02"], "key02", BlendMode::Max) == BlendMode::Max);
	CHECK(Parser::getBlendModeKey(doc["key03"], "key03") == BlendMode::Alpha);
}

TEST_CASE("Parser::getDirectionKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "left" },
	  "key02": "left",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getDirectionKey(doc["key01"], "key01") == Direction::Left);
	CHECK(Parser::getDirectionKey(doc["key02"], "key02", Direction::Right) == Direction::Right);
	CHECK(Parser::getDirectionKey(doc["key03"], "key03") == Direction::Up);
}

TEST_CASE("Parser::getBoolKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": true },
	  "key02": false,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBoolKey(doc["key01"], "key01") == true);
	CHECK(Parser::getBoolKey(doc["key02"], "key02", true) == true);
	CHECK(Parser::getBoolKey(doc["key03"], "key03") == false);
}

TEST_CASE("Parser::getDoubleKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1.0 },
	  "key02": 0.0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getDoubleKey(doc["key01"], "key01") == 1.0);
	CHECK(Parser::getDoubleKey(doc["key02"], "key02", 1.0) == 1.0);
	CHECK(Parser::getDoubleKey(doc["key03"], "key03") == 0.0);
}

TEST_CASE("Parser::getFloatKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1.0 },
	  "key02": 0.0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getFloatKey(doc["key01"], "key01") == 1.f);
	CHECK(Parser::getFloatKey(doc["key02"], "key02", 1.f) == 1.f);
	CHECK(Parser::getFloatKey(doc["key03"], "key03") == 0.f);
}

TEST_CASE("Parser::getIntKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": -1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIntKey(doc["key01"], "key01") == -1);
	CHECK(Parser::getIntKey(doc["key02"], "key02", 1) == 1);
	CHECK(Parser::getIntKey(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getInt64Key", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": -1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getInt64Key(doc["key01"], "key01") == -1);
	CHECK(Parser::getInt64Key(doc["key02"], "key02", 1) == 1);
	CHECK(Parser::getInt64Key(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getStringCharKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "text", "key02": 120 },
	  "key02": "invalid",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringCharKey(doc["key01"], "key01") == std::string("text"));
	CHECK(Parser::getStringCharKey(doc["key01"], "key02") == std::string());
	CHECK(Parser::getStringCharKey(doc["key02"], "key02", "text") == std::string("text"));
	CHECK(Parser::getStringCharKey(doc["key03"], "key03") == std::string());
}

TEST_CASE("Parser::getStringKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "text", "key02": 120 },
	  "key02": "invalid",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringKey(doc["key01"], "key01") == "text");
	CHECK(Parser::getStringKey(doc["key01"], "key02") == "120");
	CHECK(Parser::getStringKey(doc["key02"], "key02", "text") == "text");
	CHECK(Parser::getStringKey(doc["key03"], "key03") == "");
}

TEST_CASE("Parser::getStringViewKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "text", "key02": 120 },
	  "key02": "invalid",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringViewKey(doc["key01"], "key01") == "text");
	CHECK(Parser::getStringViewKey(doc["key01"], "key02") == "");
	CHECK(Parser::getStringViewKey(doc["key02"], "key02", "text") == "text");
	CHECK(Parser::getStringViewKey(doc["key03"], "key03") == "");
}

TEST_CASE("Parser::getUIntKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getUIntKey(doc["key01"], "key01") == 1);
	CHECK(Parser::getUIntKey(doc["key02"], "key02", 1) == 1);
	CHECK(Parser::getUIntKey(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getUInt64Key", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getUInt64Key(doc["key01"], "key01") == 1);
	CHECK(Parser::getUInt64Key(doc["key02"], "key02", 1) == 1);
	CHECK(Parser::getUInt64Key(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getNumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1, "key02": -1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getNumberKey<int>(doc["key01"], "key01") == 1);
	CHECK(Parser::getNumberKey<int>(doc["key01"], "key02") == -1);
	CHECK(Parser::getNumberKey<int>(doc["key02"], "key02", -1) == -1);
	CHECK(Parser::getNumberKey<int>(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getUnsignedNumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1, "key02": -1 },
	  "key02": 0,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getUnsignedNumberKey<int>(doc["key01"], "key01") == 1);
	CHECK(Parser::getUnsignedNumberKey<int>(doc["key01"], "key02") == 0);
	CHECK(Parser::getUnsignedNumberKey<int>(doc["key02"], "key02", -1) == -1);
	CHECK(Parser::getUnsignedNumberKey<int>(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getVector2NumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2] },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using VectorType = PairFloat;

		CHECK(Parser::getVector2fKey<VectorType>(doc["key01"], "key01") == VectorType(1.f, -2.f));
		CHECK(Parser::getVector2fKey<VectorType>(doc["key02"], "key02", VectorType(-1.f, 2.f)) == VectorType(-1.f, 2.f));
		CHECK(Parser::getVector2fKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int32_t")
	{
		using VectorType = PairInt32;

		CHECK(Parser::getVector2iKey<VectorType>(doc["key01"], "key01") == VectorType(1, -2));
		CHECK(Parser::getVector2iKey<VectorType>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
		CHECK(Parser::getVector2iKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("uint32_t")
	{
		using VectorType = PairUInt32;

		CHECK(Parser::getVector2uKey<VectorType>(doc["key01"], "key01") == VectorType(1, 0));
		CHECK(Parser::getVector2uKey<VectorType>(doc["key02"], "key02", VectorType(1, 2)) == VectorType(1, 2));
		CHECK(Parser::getVector2uKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::pair<Type, Type>;

		CHECK(Parser::getVector2NumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2));
		CHECK(Parser::getVector2NumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
		CHECK(Parser::getVector2NumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
}

TEST_CASE("Parser::getVector2UnsignedNumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2] },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	using Type = int32_t;
	using VectorType = PairInt32;

	CHECK(Parser::getVector2UnsignedNumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, 0));
	CHECK(Parser::getVector2UnsignedNumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
	CHECK(Parser::getVector2UnsignedNumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
}

TEST_CASE("Parser::getVector3NumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2, 3.5] },
	  "key02": [1, 2, 3],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		// same as getVector3fKey
		using Type = float;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3fKey<VectorType>(doc["key01"], "key01") == VectorType(1.f, -2.f, 3.5f));
		CHECK(Parser::getVector3fKey<VectorType>(doc["key02"], "key02", VectorType(-1.f, 2.f, 0.f)) == VectorType(-1.f, 2.f, 0.f));
		CHECK(Parser::getVector3fKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3iKey<VectorType>(doc["key01"], "key01") == VectorType(1, -2, 3));
		CHECK(Parser::getVector3iKey<VectorType>(doc["key02"], "key02", VectorType(-1, 2, 0)) == VectorType(-1, 2, 0));
		CHECK(Parser::getVector3iKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, 0, 3));
		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(1, 2, 0)) == VectorType(1, 2, 0));
		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2, 3));
		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2, 0)) == VectorType(-1, 2, 0));
		CHECK(Parser::getVector3NumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
}

TEST_CASE("Parser::getVector4NumberKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2, 3.5, 0] },
	  "key02": [1, 2, 3, 0],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		// same as getVector3fKey
		using Type = float;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4fKey<VectorType>(doc["key01"], "key01") == VectorType(1.f, -2.f, 3.5f, 0.f));
		CHECK(Parser::getVector4fKey<VectorType>(doc["key02"], "key02", VectorType(-1.f, 2.f, 0.f, 0.f)) == VectorType(-1.f, 2.f, 0.f, 0.f));
		CHECK(Parser::getVector4fKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4iKey<VectorType>(doc["key01"], "key01") == VectorType(1, -2, 3, 0));
		CHECK(Parser::getVector4iKey<VectorType>(doc["key02"], "key02", VectorType(-1, 2, 0, 0)) == VectorType(-1, 2, 0, 0));
		CHECK(Parser::getVector4iKey<VectorType>(doc["key03"], "key03") == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, 0, 3, 0));
		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2, 0, 0)) == VectorType(-1, 2, 0, 0));
		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2, 3, 0));
		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2, 0, 0)) == VectorType(-1, 2, 0, 0));
		CHECK(Parser::getVector4NumberKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
}

TEST_CASE("Parser::getRangeOrValueKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2], "key02": 1 },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	using Type = int32_t;
	using VectorType = PairInt32;

	CHECK(Parser::getRangeOrValueKey<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2));
	CHECK(Parser::getRangeOrValueKey<VectorType, Type>(doc["key01"], "key02") == VectorType(1, 1));
	CHECK(Parser::getRangeOrValueKey<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
	CHECK(Parser::getRangeOrValueKey<VectorType, Type>(doc["key03"], "key03") == VectorType());
}

TEST_CASE("Parser::getRangeNKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, -2], "key02": 1 },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	using Type = int32_t;
	using VectorType = std::tuple<Type, Type>;

	SECTION("getRange0Key")
	{
		CHECK(Parser::getRange0Key<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2));
		CHECK(Parser::getRange0Key<VectorType, Type>(doc["key01"], "key02") == VectorType(0, 1));
		CHECK(Parser::getRange0Key<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
		CHECK(Parser::getRange0Key<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
	SECTION("getRange1Key")
	{
		CHECK(Parser::getRange1Key<VectorType, Type>(doc["key01"], "key01") == VectorType(1, -2));
		CHECK(Parser::getRange1Key<VectorType, Type>(doc["key01"], "key02") == VectorType(1, 1));
		CHECK(Parser::getRange1Key<VectorType, Type>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
		CHECK(Parser::getRange1Key<VectorType, Type>(doc["key03"], "key03") == VectorType());
	}
	SECTION("getRangeNKey")
	{
		CHECK(Parser::getRangeNKey<VectorType, Type, 10>(doc["key01"], "key01") == VectorType(1, -2));
		CHECK(Parser::getRangeNKey<VectorType, Type, 10>(doc["key01"], "key02") == VectorType(10, 1));
		CHECK(Parser::getRangeNKey<VectorType, Type, 10>(doc["key02"], "key02", VectorType(-1, 2)) == VectorType(-1, 2));
		CHECK(Parser::getRangeNKey<VectorType, Type, 10>(doc["key03"], "key03") == VectorType());
	}
}

TEST_CASE("Parser::getPositionKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, "center"] },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	sf::Vector2f size(10.f, 10.f);
	sf::Vector2u refSize(320, 240);

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getPositionKey(doc["key01"], "key01", size, refSize) == sf::Vector2f(1.f, 115.f));
	CHECK(Parser::getPositionKey(doc["key02"], "key02", size, refSize) == sf::Vector2f());
	CHECK(Parser::getPositionKey(doc["key03"], "key03", size, refSize) == sf::Vector2f());
}

TEST_CASE("Parser::getSizeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, 2] },
	  "key02": [1, 2],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getSizeKey(doc["key01"], "key01") == sf::Vector2f(1.f, 2.f));
	CHECK(Parser::getSizeKey(doc["key02"], "key02", sf::Vector2f(-1.f, 2.f)) == sf::Vector2f(-1.f, 2.f));
	CHECK(Parser::getSizeKey(doc["key03"], "key03") == sf::Vector2f());
}

TEST_CASE("Parser::getIntRectKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, 2, 3, 4] },
	  "key02": [1, 2, 3, 4],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIntRectKey(doc["key01"], "key01") == sf::IntRect(1, 2, 3, 4));
	CHECK(Parser::getIntRectKey(doc["key02"], "key02", sf::IntRect(-1, 2, 3, 4)) == sf::IntRect(-1, 2, 3, 4));
	CHECK(Parser::getIntRectKey(doc["key03"], "key03") == sf::IntRect());
}

TEST_CASE("Parser::getFloatRectKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": [1, 2, 3, 4] },
	  "key02": [1, 2, 3, 4],
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getFloatRectKey(doc["key01"], "key01") == sf::FloatRect(1.f, 2.f, 3.f, 4.f));
	CHECK(Parser::getFloatRectKey(doc["key02"], "key02", sf::FloatRect(-1.f, 2.f, 3.f, 4.f)) == sf::FloatRect(-1.f, 2.f, 3.f, 4.f));
	CHECK(Parser::getFloatRectKey(doc["key03"], "key03") == sf::FloatRect());
}

TEST_CASE("Parser::getColorKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "0x0000FF" },
	  "key02": "0x0000FF",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getColorKey(doc["key01"], "key01") == sf::Color::Blue);
	CHECK(Parser::getColorKey(doc["key02"], "key02", sf::Color::Red) == sf::Color::Red);
	CHECK(Parser::getColorKey(doc["key03"], "key03") == sf::Color::Black);
}

TEST_CASE("Parser::getTimeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1200, "key02": -1200 },
	  "key02": 1200,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getTimeKey(doc["key01"], "key01") == sf::milliseconds(1200));
	CHECK(Parser::getTimeKey(doc["key01"], "key02") == sf::milliseconds(-1200));
	CHECK(Parser::getTimeKey(doc["key02"], "key02", sf::seconds(1.f)) == sf::seconds(1.f));
	CHECK(Parser::getTimeKey(doc["key03"], "key03") == sf::Time::Zero);
}

TEST_CASE("Parser::getTimeUKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": 1200, "key02": -1200 },
	  "key02": 1200,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getTimeUKey(doc["key01"], "key01") == sf::milliseconds(1200));
	CHECK(Parser::getTimeUKey(doc["key01"], "key02") == sf::Time::Zero);
	CHECK(Parser::getTimeUKey(doc["key02"], "key02", sf::seconds(1.f)) == sf::seconds(1.f));
	CHECK(Parser::getTimeUKey(doc["key03"], "key03") == sf::Time::Zero);
}

TEST_CASE("Parser::getStringVectorKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": ["this", "is", 1, true, "string", 9.9]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	std::vector<std::string> vec = { "this", "is", "1", "true", "string", "9.9" };

	CHECK(Parser::getStringVectorKey(doc, "key01") == vec);

	vec.clear();

	CHECK(Parser::getStringVectorKey(doc, "key02") == vec);
}

TEST_CASE("Parser::getIgnoreResourceKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "draw", "key02": ["draw", "update"] },
	  "key02": "draw",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIgnoreResourceKey(doc["key01"], "key01") == IgnoreResource::Draw);
	CHECK(Parser::getIgnoreResourceKey(doc["key01"], "key02") == (IgnoreResource::Draw | IgnoreResource::Update));
	CHECK(Parser::getIgnoreResourceKey(doc["key02"], "key02", IgnoreResource::Update) == IgnoreResource::Update);
	CHECK(Parser::getIgnoreResourceKey(doc["key03"], "key03") == IgnoreResource::None);
}

TEST_CASE("Parser::getInputEventTypeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "mousePress", "key02": ["mousePress", "mouseRelease"] },
	  "key02": "draw",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getInputEventTypeKey(doc["key01"], "key01") == InputEventType::MousePress);
	CHECK(Parser::getInputEventTypeKey(doc["key01"], "key02") == (InputEventType::MousePress | InputEventType::MouseRelease));
	CHECK(Parser::getInputEventTypeKey(doc["key02"], "key02", InputEventType::LeftClick) == InputEventType::LeftClick);
	CHECK(Parser::getInputEventTypeKey(doc["key03"], "key03") == InputEventType::None);
}

TEST_CASE("Parser::getMinMaxIntKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "max", "key02": 1 },
	  "key02": 1,
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	using Type = int;

	CHECK(Parser::getMinMaxIntKey<Type>(doc["key01"], "key01") == std::numeric_limits<Type>::max());
	CHECK(Parser::getMinMaxIntKey<Type>(doc["key01"], "key02") == 1);
	CHECK(Parser::getMinMaxIntKey<Type>(doc["key02"], "key02", 2) == 2);
	CHECK(Parser::getMinMaxIntKey<Type>(doc["key03"], "key03") == 0);
}

TEST_CASE("Parser::getPrimitiveTypeKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "points" },
	  "key02": "points",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getPrimitiveTypeKey(doc["key01"], "key01") == sf::PrimitiveType::Points);
	CHECK(Parser::getPrimitiveTypeKey(doc["key02"], "key02", sf::PrimitiveType::Triangles) == sf::PrimitiveType::Triangles);
	CHECK(Parser::getPrimitiveTypeKey(doc["key03"], "key03") == sf::PrimitiveType::TriangleStrip);
}

TEST_CASE("Parser::getQueryKey", "Parser")
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
	  "query02": "",
	  "query03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getQueryKey(doc["document"], doc, "query01") == doc["document"]["key01"]["subKey01"]);
	CHECK(Parser::getQueryKey(doc["document"], doc, "query02") == doc["query02"]);
	CHECK(Parser::getQueryKey(doc["document"], doc, "query03") == doc["query03"]);
	CHECK(Parser::getQueryKey(doc["document"], doc, "query04").IsNull() == true);
}

TEST_CASE("Parser::getReplaceVarsKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "string" },
	  "key02": "string",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getReplaceVarsKey(doc["key01"], "key01") == ReplaceVars::String);
	CHECK(Parser::getReplaceVarsKey(doc["key02"], "key02", ReplaceVars::Value) == ReplaceVars::Value);
	CHECK(Parser::getReplaceVarsKey(doc["key03"], "key03") == ReplaceVars::None);
}

TEST_CASE("Parser::getVariableKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "string" },
	  "key02": "string",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getVariableKey(doc["key01"], "key01") == Variable("string"));
	CHECK(Parser::getVariableKey(doc["key02"], "key02") == Variable());
	CHECK(Parser::getVariableKey(doc["key03"], "key03") == Variable());
}

TEST_CASE("Parser::getVariablesKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "id": "id1", "value": 10 },
	  "key02": "string",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	std::vector<std::pair<std::string, Variable>> vars =
	{
		{ std::string("id"), Variable("id1") },
		{ std::string("value"), Variable(10) }
	};

	CHECK(Parser::getVariablesKey(doc, "key01") == vars);

	vars.clear();

	CHECK(Parser::getVariablesKey(doc, "key02") == vars);
	CHECK(Parser::getVariablesKey(doc, "key03") == vars);
	CHECK(Parser::getVariablesKey(doc, "key04") == vars);
}

TEST_CASE("Parser::getVariablesMapKey", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "id": "id1", "value": 10 },
	  "key02": "string",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	UnorderedStringMap<Variable> vars =
	{
		{ std::string("id"), Variable("id1") },
		{ std::string("value"), Variable(10) }
	};

	CHECK(Parser::getVariablesMapKey(doc, "key01") == vars);

	vars.clear();

	CHECK(Parser::getVariablesMapKey(doc, "key02") == vars);
	CHECK(Parser::getVariablesMapKey(doc, "key03") == vars);
	CHECK(Parser::getVariablesMapKey(doc, "key04") == vars);
}

TEST_CASE("Parser::getVarOrPredicateKey", "Parser")
{
	Game game;
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": { "key01": "string", "key02": { "name": "file.exists", "file": "text.txt" } },
	  "key02": "string",
	  "key03": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	auto var = Parser::getVarOrPredicateKey(game, doc["key01"], "key01");

	CHECK(std::holds_alternative<Variable>(var));
	CHECK(std::get<Variable>(var) == Variable("string"));

	var = Parser::getVarOrPredicateKey(game, doc["key01"], "key02");

	CHECK(holdsPredicate(var) == true);
	CHECK(std::get<std::shared_ptr<Predicate>>(var) != nullptr);

	var = Parser::getVarOrPredicateKey(game, doc["key02"], "key02");

	CHECK(std::holds_alternative<Variable>(var));
	CHECK(std::get<Variable>(var) == Variable());

	var = Parser::getVarOrPredicateKey(game, doc["key03"], "key03");

	CHECK(std::holds_alternative<Variable>(var));
	CHECK(std::get<Variable>(var) == Variable());
}
