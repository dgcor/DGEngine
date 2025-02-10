#include "catch2/catch_test_macros.hpp"

#include "Game/Game.h"
#include "Game/Utils/VarUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/Utils/ParseUtilsVal.h"

TEST_CASE("Parser::getAnchorVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "all",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": ["all"],
	  "key06": ["top", "right"],
	  "key07": ["none", "all"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getAnchorVal(doc["key01"]) == Anchor::All);
	CHECK(Parser::getAnchorVal(doc["key02"], Anchor::All) == Anchor::All);
	CHECK(Parser::getAnchorVal(doc["key03"]) == (Anchor::Top | Anchor::Left));
	CHECK(Parser::getAnchorVal(doc["key04"]) == (Anchor::Top | Anchor::Left));
	CHECK(Parser::getAnchorVal(doc["key05"]) == Anchor::All);
	CHECK(Parser::getAnchorVal(doc["key06"]) == (Anchor::Top | Anchor::Right));
	CHECK(Parser::getAnchorVal(doc["key07"]) == Anchor::All);
}

TEST_CASE("Parser::getAnimationTypeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "playOnce",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": ["playOnce"],
	  "key06": ["playOnce", "looped"],
	  "key07": ["none", "playOnce"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getAnimationTypeVal(doc["key01"]) == AnimationType::PlayOnce);
	CHECK(Parser::getAnimationTypeVal(doc["key02"], AnimationType::BackAndForth) == AnimationType::BackAndForth);
	CHECK(Parser::getAnimationTypeVal(doc["key03"]) == AnimationType::Looped);
	CHECK(Parser::getAnimationTypeVal(doc["key04"]) == AnimationType::Looped);
	CHECK(Parser::getAnimationTypeVal(doc["key05"]) == AnimationType::Looped);
	CHECK(Parser::getAnimationTypeVal(doc["key06"]) == AnimationType::Looped);
	CHECK(Parser::getAnimationTypeVal(doc["key07"]) == AnimationType::Looped);
}

TEST_CASE("Parser::getBindingFlagsVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "always",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": ["always"],
	  "key06": ["always", "whenhidden"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBindingFlagsVal(doc["key01"]) == BindingFlags::Always);
	CHECK(Parser::getBindingFlagsVal(doc["key02"], BindingFlags::Always) == BindingFlags::Always);
	CHECK(Parser::getBindingFlagsVal(doc["key03"]) == BindingFlags::OnChange);
	CHECK(Parser::getBindingFlagsVal(doc["key04"]) == BindingFlags::OnChange);
	CHECK(Parser::getBindingFlagsVal(doc["key05"]) == BindingFlags::Always);
	CHECK(Parser::getBindingFlagsVal(doc["key06"]) == (BindingFlags::Always | BindingFlags::WhenHidden));
}

TEST_CASE("Parser::getBlendModeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "add",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBlendModeVal(doc["key01"]) == BlendMode::Add);
	CHECK(Parser::getBlendModeVal(doc["key02"], BlendMode::None) == BlendMode::None);
	CHECK(Parser::getBlendModeVal(doc["key03"]) == BlendMode::Alpha);
	CHECK(Parser::getBlendModeVal(doc["key04"]) == BlendMode::Alpha);
}

TEST_CASE("Parser::getDirectionVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "down",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getDirectionVal(doc["key01"]) == Direction::Down);
	CHECK(Parser::getDirectionVal(doc["key02"], Direction::Down) == Direction::Down);
	CHECK(Parser::getDirectionVal(doc["key03"]) == Direction::Up);
	CHECK(Parser::getDirectionVal(doc["key04"]) == Direction::Up);
}

TEST_CASE("Parser::getBoolVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": true,
	  "key02": "invalid",
	  "key03": "",
	  "key04": 1
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getBoolVal(doc["key01"]) == true);
	CHECK(Parser::getBoolVal(doc["key02"], true) == true);
	CHECK(Parser::getBoolVal(doc["key03"]) == false);
	CHECK(Parser::getBoolVal(doc["key04"]) == false);
}

TEST_CASE("Parser::getDoubleVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 1.2,
	  "key02": -1.2,
	  "key03": 9000000000.5,
	  "key04": "invalid",
	  "key05": "1.2",
	  "key06": 1,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getDoubleVal(doc["key01"]) == 1.2);
	CHECK(Parser::getDoubleVal(doc["key02"]) == -1.2);
	CHECK(Parser::getDoubleVal(doc["key03"]) == 9000000000.5);
	CHECK(Parser::getDoubleVal(doc["key04"], 1.2) == 1.2);
	CHECK(Parser::getDoubleVal(doc["key05"]) == 0.0);
	CHECK(Parser::getDoubleVal(doc["key06"]) == 0.0);
	CHECK(Parser::getDoubleVal(doc["key07"]) == 0.0);
}

TEST_CASE("Parser::getFloatVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 1.2,
	  "key02": -1.2,
	  "key03": 9000000000.5,
	  "key04": "invalid",
	  "key05": "1.2",
	  "key06": 1,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getFloatVal(doc["key01"]) == 1.2f);
	CHECK(Parser::getFloatVal(doc["key02"]) == -1.2f);
	CHECK(Parser::getFloatVal(doc["key03"]) == 0.f);
	CHECK(Parser::getFloatVal(doc["key04"], 1.2f) == 1.2f);
	CHECK(Parser::getFloatVal(doc["key05"]) == 0.f);
	CHECK(Parser::getFloatVal(doc["key06"]) == 0.f);
	CHECK(Parser::getFloatVal(doc["key07"]) == 0.f);
}

TEST_CASE("Parser::getIntVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": "invalid",
	  "key05": "1",
	  "key06": 1.2,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIntVal(doc["key01"]) == 10);
	CHECK(Parser::getIntVal(doc["key02"]) == -10);
	CHECK(Parser::getIntVal(doc["key03"]) == 0);
	CHECK(Parser::getIntVal(doc["key04"], 10) == 10);
	CHECK(Parser::getIntVal(doc["key05"]) == 0);
	CHECK(Parser::getIntVal(doc["key06"]) == 0);
	CHECK(Parser::getIntVal(doc["key07"]) == 0);
}

TEST_CASE("Parser::getInt64Val", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": "invalid",
	  "key05": "1",
	  "key06": 1.2,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getInt64Val(doc["key01"]) == 10);
	CHECK(Parser::getInt64Val(doc["key02"]) == -10);
	CHECK(Parser::getInt64Val(doc["key03"]) == 9000000000);
	CHECK(Parser::getInt64Val(doc["key04"], 10) == 10);
	CHECK(Parser::getInt64Val(doc["key05"]) == 0);
	CHECK(Parser::getInt64Val(doc["key06"]) == 0);
	CHECK(Parser::getInt64Val(doc["key07"]) == 0);
}

TEST_CASE("Parser::getStringCharVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": "",
	  "key03": -10,
	  "key04": 1.2,
	  "key05": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringCharVal(doc["key01"]) == std::string("text"));
	CHECK(Parser::getStringCharVal(doc["key02"], "text") == std::string(""));
	CHECK(Parser::getStringCharVal(doc["key03"], "text") == std::string("text"));
	CHECK(Parser::getStringCharVal(doc["key04"]) == std::string(""));
	CHECK(Parser::getStringCharVal(doc["key05"]) == std::string(""));
}

TEST_CASE("Parser::getStringVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": "",
	  "key03": -10,
	  "key04": 1.2,
	  "key05": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringVal(doc["key01"]) == std::string("text"));
	CHECK(Parser::getStringVal(doc["key02"], "text") == std::string(""));
	CHECK(Parser::getStringVal(doc["key03"]) == std::string("-10"));
	CHECK(Parser::getStringVal(doc["key04"]) == std::string("1.2"));
	CHECK(Parser::getStringVal(doc["key05"]) == std::string(""));
}

TEST_CASE("Parser::getStringViewVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "text",
	  "key02": "",
	  "key03": -10,
	  "key04": 1.2,
	  "key05": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getStringViewVal(doc["key01"]) == std::string("text"));
	CHECK(Parser::getStringViewVal(doc["key02"], "text") == std::string(""));
	CHECK(Parser::getStringViewVal(doc["key03"], "text") == std::string("text"));
	CHECK(Parser::getStringViewVal(doc["key04"]) == std::string(""));
	CHECK(Parser::getStringViewVal(doc["key05"]) == std::string(""));
}

TEST_CASE("Parser::getUIntVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": "invalid",
	  "key05": "1",
	  "key06": 1.2,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getUIntVal(doc["key01"]) == 10);
	CHECK(Parser::getUIntVal(doc["key02"]) == 0);
	CHECK(Parser::getUIntVal(doc["key03"]) == 0);
	CHECK(Parser::getUIntVal(doc["key04"], 10) == 10);
	CHECK(Parser::getUIntVal(doc["key05"]) == 0);
	CHECK(Parser::getUIntVal(doc["key06"]) == 0);
	CHECK(Parser::getUIntVal(doc["key07"]) == 0);
}

TEST_CASE("Parser::getUInt64Val", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": "invalid",
	  "key05": "1",
	  "key06": 1.2,
	  "key07": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getUInt64Val(doc["key01"]) == 10);
	CHECK(Parser::getUInt64Val(doc["key02"]) == 0);
	CHECK(Parser::getUInt64Val(doc["key03"]) == 9000000000);
	CHECK(Parser::getUInt64Val(doc["key04"], 10) == 10);
	CHECK(Parser::getUInt64Val(doc["key05"]) == 0);
	CHECK(Parser::getUInt64Val(doc["key06"]) == 0);
	CHECK(Parser::getUInt64Val(doc["key07"]) == 0);
}

TEST_CASE("Parser::getNumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": 1.2,
	  "key05": 9000000000.5,
	  "key06": "invalid",
	  "key07": "1",
	  "key08": true,
	  "key09": 0,
	  "key10": 0.0
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("int32_t")
	{
		CHECK(Parser::getNumberVal<int32_t>(doc["key01"]) == 10);
		CHECK(Parser::getNumberVal<int32_t>(doc["key02"]) == -10);
		CHECK(Parser::getNumberVal<int32_t>(doc["key03"]) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key04"]) == 1);
		CHECK(Parser::getNumberVal<int32_t>(doc["key05"]) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key06"]) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key07"]) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key08"]) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getNumberVal<int32_t>(doc["key10"], 1) == 0);
	}
	SECTION("int64_t")
	{
		CHECK(Parser::getNumberVal<int64_t>(doc["key01"]) == 10);
		CHECK(Parser::getNumberVal<int64_t>(doc["key02"]) == -10);
		CHECK(Parser::getNumberVal<int64_t>(doc["key03"]) == 9000000000);
		CHECK(Parser::getNumberVal<int64_t>(doc["key04"]) == 1);
		CHECK(Parser::getNumberVal<int64_t>(doc["key05"]) == 9000000000);
		CHECK(Parser::getNumberVal<int64_t>(doc["key06"]) == 0);
		CHECK(Parser::getNumberVal<int64_t>(doc["key07"]) == 0);
		CHECK(Parser::getNumberVal<int64_t>(doc["key08"]) == 0);
		CHECK(Parser::getNumberVal<int64_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getNumberVal<int64_t>(doc["key10"], 1) == 0);
	}
	SECTION("uint32_t")
	{
		CHECK(Parser::getNumberVal<uint32_t>(doc["key01"]) == 10);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key02"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key03"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key04"]) == 1);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key05"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key06"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key07"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key08"]) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getNumberVal<uint32_t>(doc["key10"], 1) == 0);
	}
	SECTION("uint64_t")
	{
		CHECK(Parser::getNumberVal<uint64_t>(doc["key01"]) == 10);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key02"]) == 0);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key03"]) == 9000000000);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key04"]) == 1);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key05"]) == 9000000000);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key06"]) == 0);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key07"]) == 0);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key08"]) == 0);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getNumberVal<uint64_t>(doc["key10"], 1) == 0);
	}
	SECTION("float")
	{
		CHECK(Parser::getNumberVal<float>(doc["key01"]) == 10.f);
		CHECK(Parser::getNumberVal<float>(doc["key02"]) == -10.f);
		CHECK(Parser::getNumberVal<float>(doc["key03"]) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key04"]) == 1.2f);
		CHECK(Parser::getNumberVal<float>(doc["key05"]) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key06"]) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key07"]) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key08"]) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key09"], 1.0) == 0.f);
		CHECK(Parser::getNumberVal<float>(doc["key10"], 1.0) == 0.f);
	}
	SECTION("double")
	{
		CHECK(Parser::getNumberVal<double>(doc["key01"]) == 10.0);
		CHECK(Parser::getNumberVal<double>(doc["key02"]) == -10.0);
		CHECK(Parser::getNumberVal<double>(doc["key03"]) == 9000000000.0);
		CHECK(Parser::getNumberVal<double>(doc["key04"]) == 1.2);
		CHECK(Parser::getNumberVal<double>(doc["key05"]) == 9000000000.5);
		CHECK(Parser::getNumberVal<double>(doc["key06"]) == 0.0);
		CHECK(Parser::getNumberVal<double>(doc["key07"]) == 0.0);
		CHECK(Parser::getNumberVal<double>(doc["key08"]) == 0.0);
		CHECK(Parser::getNumberVal<double>(doc["key09"], 1.0) == 0.f);
		CHECK(Parser::getNumberVal<double>(doc["key10"], 1.0) == 0.f);
	}
}

TEST_CASE("Parser::getUnsignedNumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": -10,
	  "key03": 9000000000,
	  "key04": 1.2,
	  "key05": 9000000000.5,
	  "key06": "invalid",
	  "key07": "1",
	  "key08": true,
	  "key09": 0,
	  "key10": 0.0
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("int32_t")
	{
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key01"]) == 10);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key02"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key03"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key04"]) == 1);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key05"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key06"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key07"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key08"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getUnsignedNumberVal<int32_t>(doc["key10"], 1) == 0);
	}
	SECTION("int64_t")
	{
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key01"]) == 10);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key02"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key03"]) == 9000000000);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key04"]) == 1);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key05"]) == 9000000000);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key06"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key07"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key08"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getUnsignedNumberVal<int64_t>(doc["key10"], 1) == 0);
	}
	SECTION("uint32_t")
	{
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key01"]) == 10);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key02"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key03"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key04"]) == 1);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key05"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key06"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key07"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key08"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint32_t>(doc["key10"], 1) == 0);
	}
	SECTION("uint64_t")
	{
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key01"]) == 10);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key02"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key03"]) == 9000000000);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key04"]) == 1);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key05"]) == 9000000000);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key06"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key07"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key08"]) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key09"], 1) == 0);
		CHECK(Parser::getUnsignedNumberVal<uint64_t>(doc["key10"], 1) == 0);
	}
	SECTION("float")
	{
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key01"]) == 10.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key02"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key03"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key04"]) == 1.2f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key05"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key06"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key07"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key08"]) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key09"], 1.0) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<float>(doc["key10"], 1.0) == 0.f);
	}
	SECTION("double")
	{
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key01"]) == 10.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key02"]) == 0.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key03"]) == 9000000000.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key04"]) == 1.2);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key05"]) == 9000000000.5);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key06"]) == 0.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key07"]) == 0.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key08"]) == 0.0);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key09"], 1.0) == 0.f);
		CHECK(Parser::getUnsignedNumberVal<double>(doc["key10"], 1.0) == 0.f);
	}
}

TEST_CASE("Parser::getVector2NumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [10, -20],
	  "key02": [400, 9000000000],
	  "key03": [1.2, -2.9, 9.0, 9.0],
	  "key04": [1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using VectorType = PairFloat;

		CHECK(Parser::getVector2fVal<VectorType>(doc["key01"]) == VectorType(10.f, -20.f));
		CHECK(Parser::getVector2fVal<VectorType>(doc["key02"]) == VectorType(400.f, 0.f));
		CHECK(Parser::getVector2fVal<VectorType>(doc["key03"]) == VectorType(1.2f, -2.9f));
		CHECK(Parser::getVector2fVal<VectorType>(doc["key04"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getVector2fVal<VectorType>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2fVal<VectorType>(doc["key06"], VectorType(1.f, 1.f)) == VectorType(1.f, 1.f));
		CHECK(Parser::getVector2fVal<VectorType>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2fVal<VectorType>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using VectorType = PairInt32;

		CHECK(Parser::getVector2iVal<VectorType>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getVector2iVal<VectorType>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getVector2iVal<VectorType>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getVector2iVal<VectorType>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getVector2iVal<VectorType>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2iVal<VectorType>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2iVal<VectorType>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2iVal<VectorType>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using VectorType = PairUInt32;

		CHECK(Parser::getVector2uVal<VectorType>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getVector2uVal<VectorType>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getVector2uVal<VectorType>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getVector2uVal<VectorType>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getVector2uVal<VectorType>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2uVal<VectorType>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2uVal<VectorType>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2uVal<VectorType>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::pair<Type, Type>;

		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key02"]) == VectorType(400, 9000000000));
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key04"]) == VectorType(1, 9000000000));
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getVector2UnsignedNumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [10, -20],
	  "key02": [400, 9000000000],
	  "key03": [1.2, -2.9, 9.0, 9.0],
	  "key04": [1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = PairFloat;

		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key01"]) == VectorType(10.f, 0.f));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key02"]) == VectorType(400.f, 0.f));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key03"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key04"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f)) == VectorType(1.f, 1.f));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = PairInt32;

		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = PairUInt32;

		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::pair<Type, Type>;

		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key02"]) == VectorType(400, 9000000000));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key04"]) == VectorType(1, 9000000000));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector2UnsignedNumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getVector3NumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 10, -20],
	  "key02": [0, 400, 9000000000],
	  "key03": [0.0, 1.2, -2.9, 9.0, 9.0],
	  "key04": [0.0, 1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2, 0],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0.f, 10.f, -20.f));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0.f, 400.f, 0.f));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0.f, 1.2f, -2.9f));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0.f, 1.2f, 0.f));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f, 1.f)) == VectorType(1.f, 1.f, 1.f));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 10, -20));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 400, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 1, -2));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 1, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1)) == VectorType(1, 1, 1));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 10, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 400, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 1, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 1, 0));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1)) == VectorType(1, 1, 1));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type, Type>;

		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 10, -20));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 400, 9000000000));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 1, -2));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 1, 9000000000));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1)) == VectorType(1, 1, 1));
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector3NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getVector4NumberVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 0, 10, -20],
	  "key02": [0, 0, 400, 9000000000],
	  "key03": [0.0, 0.0, 1.2, -2.9, 9.0, 9.0],
	  "key04": [0.0, 0.0, 1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2, 0, 0],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0.f, 0.f, 10.f, -20.f));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0.f, 0.f, 400.f, 0.f));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0.f, 0.f, 1.2f, -2.9f));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0.f, 0.f, 1.2f, 0.f));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f, 1.f, 1.f)) == VectorType(1.f, 1.f, 1.f, 1.f));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 0, 10, -20));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 0, 400, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 0, 1, -2));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 0, 1, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1, 1)) == VectorType(1, 1, 1, 1));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 0, 10, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 0, 400, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 0, 1, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 0, 1, 0));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1, 1)) == VectorType(1, 1, 1, 1));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type, Type, Type>;

		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key01"]) == VectorType(0, 0, 10, -20));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key02"]) == VectorType(0, 0, 400, 9000000000));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key03"]) == VectorType(0, 0, 1, -2));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key04"]) == VectorType(0, 0, 1, 9000000000));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key05"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key06"], VectorType(1, 1, 1, 1)) == VectorType(1, 1, 1, 1));
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getVector4NumberVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getRangeOrValueVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [10, -20],
	  "key02": [400, 9000000000],
	  "key03": [1.2, -2.9, 9.0, 9.0],
	  "key04": [1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key01"]) == VectorType(10.f, -20.f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key02"]) == VectorType(400.f, 0.f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key03"]) == VectorType(1.2f, -2.9f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key04"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key05"]) == VectorType(1.f, 1.f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f)) == VectorType(1.f, 1.f));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key05"]) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key05"]) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key02"]) == VectorType(400, 9000000000));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key04"]) == VectorType(1, 9000000000));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key05"]) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRangeOrValueVal<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getRange0Val", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [10, -20],
	  "key02": [400, 9000000000],
	  "key03": [1.2, -2.9, 9.0, 9.0],
	  "key04": [1.2, 9000000000.5],
	  "key05": 1,
	  "key06": "invalid",
	  "key07": ["1", 2],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key01"]) == VectorType(10.f, -20.f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key02"]) == VectorType(400.f, 0.f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key03"]) == VectorType(1.2f, -2.9f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key04"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key05"]) == VectorType(0.f, 1.f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f)) == VectorType(1.f, 1.f));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key05"]) == VectorType(0, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key05"]) == VectorType(0, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key02"]) == VectorType(400, 9000000000));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key04"]) == VectorType(1, 9000000000));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key05"]) == VectorType(0, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange0Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getRange1Val", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [10, -20],
	  "key02": [400, 9000000000],
	  "key03": [1.2, -2.9, 9.0, 9.0],
	  "key04": [1.2, 9000000000.5],
	  "key05": 2,
	  "key06": "invalid",
	  "key07": ["1", 2],
	  "key08": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("float")
	{
		using Type = float;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key01"]) == VectorType(10.f, -20.f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key02"]) == VectorType(400.f, 0.f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key03"]) == VectorType(1.2f, -2.9f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key04"]) == VectorType(1.2f, 0.f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key05"]) == VectorType(1.f, 2.f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key06"], VectorType(1.f, 1.f)) == VectorType(1.f, 1.f));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int32_t")
	{
		using Type = int32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key05"]) == VectorType(1, 2));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key01"]) == VectorType(10, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key02"]) == VectorType(400, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key03"]) == VectorType(1, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key04"]) == VectorType(1, 0));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key05"]) == VectorType(1, 2));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
	SECTION("int64_t")
	{
		using Type = int64_t;
		using VectorType = std::tuple<Type, Type>;

		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key01"]) == VectorType(10, -20));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key02"]) == VectorType(400, 9000000000));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key03"]) == VectorType(1, -2));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key04"]) == VectorType(1, 9000000000));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key05"]) == VectorType(1, 2));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key06"], VectorType(1, 1)) == VectorType(1, 1));
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key07"]) == VectorType());
		CHECK(Parser::getRange1Val<VectorType, Type>(doc["key08"]) == VectorType());
	}
}

TEST_CASE("Parser::getPositionVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 0],
	  "key02": [10, -100],
	  "key03": [320, 240],
	  "key04": ["left", "bottom"],
	  "key05": ["center", "center"],
	  "key06": ["right", "top"],
	  "key07": ["invalid", "text"],
	  "key08": [10.4, -10.5]
	})"
	};

	sf::Vector2f size(10.f, 10.f);
	sf::Vector2u refSize(320, 240);

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getPositionVal(doc["key01"], size, refSize) == sf::Vector2f(0.f, 0.f));
	CHECK(Parser::getPositionVal(doc["key02"], size, refSize) == sf::Vector2f(10.f, -100.f));
	CHECK(Parser::getPositionVal(doc["key03"], size, refSize) == sf::Vector2f(320.f, 240.f));
	CHECK(Parser::getPositionVal(doc["key04"], size, refSize) == sf::Vector2f(0.f, 230.f));
	CHECK(Parser::getPositionVal(doc["key05"], size, refSize) == sf::Vector2f(155.f, 115.f));
	CHECK(Parser::getPositionVal(doc["key06"], size, refSize) == sf::Vector2f(310.f, 0.f));
	CHECK(Parser::getPositionVal(doc["key07"], size, refSize) == sf::Vector2f(0.f, 0.f));
	CHECK(Parser::getPositionVal(doc["key08"], size, refSize) == sf::Vector2f(10.f, -11.f));
}

TEST_CASE("Parser::getSizeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 0],
	  "key02": [10, -100],
	  "key03": [320, 240],
	  "key04": ["left", "bottom"],
	  "key05": ["center", "center"],
	  "key06": ["right", "top"],
	  "key07": ["invalid", "text"],
	  "key08": [10.4, -10.5]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getSizeVal(doc["key01"]) == sf::Vector2f(0.f, 0.f));
	CHECK(Parser::getSizeVal(doc["key02"]) == sf::Vector2f(10.f, -100.f));
	CHECK(Parser::getSizeVal(doc["key03"]) == sf::Vector2f(320.f, 240.f));
	CHECK(Parser::getSizeVal(doc["key04"]) == sf::Vector2f());
	CHECK(Parser::getSizeVal(doc["key05"]) == sf::Vector2f());
	CHECK(Parser::getSizeVal(doc["key06"]) == sf::Vector2f());
	CHECK(Parser::getSizeVal(doc["key07"]) == sf::Vector2f());
	CHECK(Parser::getSizeVal(doc["key08"]) == sf::Vector2f(10.f, -11.f));
}

TEST_CASE("Parser::getIntRectVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 0, 10, -20],
	  "key02": [0, 0, 400, 9000000000],
	  "key03": [0.0, 0.0, 1.2, -2.9, 9.0, 9.0],
	  "key04": [0.0, 0.0, 1.2, 9000000000.5],
	  "key05": [10, 20],
	  "key06": [10.5, 20.5],
	  "key07": 1,
	  "key08": "invalid",
	  "key09": ["1", 2],
	  "key10": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIntRectVal(doc["key01"]) == sf::IntRect(0, 0, 10, -20));
	CHECK(Parser::getIntRectVal(doc["key02"]) == sf::IntRect(0, 0, 400, 0));
	CHECK(Parser::getIntRectVal(doc["key03"]) == sf::IntRect(0, 0, 1, -2));
	CHECK(Parser::getIntRectVal(doc["key04"]) == sf::IntRect(0, 0, 1, 0));
	CHECK(Parser::getIntRectVal(doc["key05"]) == sf::IntRect(0, 0, 10, 20));
	CHECK(Parser::getIntRectVal(doc["key06"]) == sf::IntRect(0, 0, 10, 20));
	CHECK(Parser::getIntRectVal(doc["key07"]) == sf::IntRect());
	CHECK(Parser::getIntRectVal(doc["key08"], sf::IntRect(1, 1, 1, 1)) == sf::IntRect(1, 1, 1, 1));
	CHECK(Parser::getIntRectVal(doc["key09"]) == sf::IntRect(0, 0, 0, 2));
	CHECK(Parser::getIntRectVal(doc["key10"]) == sf::IntRect());
}

TEST_CASE("Parser::getFloatRectVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [0, 0, 10, -20],
	  "key02": [0, 0, 400, 9000000000],
	  "key03": [0.0, 0.0, 1.2, -2.9, 9.0, 9.0],
	  "key04": [0.0, 0.0, 1.2, 9000000000.5],
	  "key05": [10, 20],
	  "key06": [10.5, 20.5],
	  "key07": 1,
	  "key08": "invalid",
	  "key09": ["1", 2],
	  "key10": [1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getFloatRectVal(doc["key01"]) == sf::FloatRect(0.f, 0.f, 10.f, -20.f));
	CHECK(Parser::getFloatRectVal(doc["key02"]) == sf::FloatRect(0.f, 0.f, 400.f, 0.f));
	CHECK(Parser::getFloatRectVal(doc["key03"]) == sf::FloatRect(0.f, 0.f, 1.2f, -2.9f));
	CHECK(Parser::getFloatRectVal(doc["key04"]) == sf::FloatRect(0.f, 0.f, 1.2f, 0.f));
	CHECK(Parser::getFloatRectVal(doc["key05"]) == sf::FloatRect(0.f, 0.f, 10.f, 20.f));
	CHECK(Parser::getFloatRectVal(doc["key06"]) == sf::FloatRect(0.f, 0.f, 10.5f, 20.5f));
	CHECK(Parser::getFloatRectVal(doc["key07"]) == sf::FloatRect());
	CHECK(Parser::getFloatRectVal(doc["key08"], sf::FloatRect(1.f, 1.f, 1.f, 1.f)) == sf::FloatRect(1.f, 1.f, 1.f, 1.f));
	CHECK(Parser::getFloatRectVal(doc["key09"]) == sf::FloatRect(0.f, 0.f, 0.f, 2.f));
	CHECK(Parser::getFloatRectVal(doc["key10"]) == sf::FloatRect());
}

TEST_CASE("Parser::getColorVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "0xAABB22",
	  "key02": "0xAABB2299",
	  "key03": 2863311615,
	  "key04": [255, 0, 128],
	  "key05": [255, 0, 128, 200, 120],
	  "key06": [255, 0],
	  "key07": "invalid",
	  "key08": "",
	  "key09": true,
	  "key10": ["10", "20", "30"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getColorVal(doc["key01"]) == sf::Color(0xAA, 0xBB, 0x22));
	CHECK(Parser::getColorVal(doc["key02"]) == sf::Color(0xAA, 0xBB, 0x22, 0x99));
	CHECK(Parser::getColorVal(doc["key03"]) == sf::Color(0xAA, 0xAA, 0xAA));
	CHECK(Parser::getColorVal(doc["key04"]) == sf::Color(255, 0, 128));
	CHECK(Parser::getColorVal(doc["key05"]) == sf::Color(255, 0, 128, 200));
	CHECK(Parser::getColorVal(doc["key06"], sf::Color::Blue) == sf::Color::Blue);
	CHECK(Parser::getColorVal(doc["key07"]) == sf::Color());
	CHECK(Parser::getColorVal(doc["key08"]) == sf::Color());
	CHECK(Parser::getColorVal(doc["key09"]) == sf::Color());
	CHECK(Parser::getColorVal(doc["key10"]) == sf::Color());
}

TEST_CASE("Parser::getKeyCodeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 0,
	  "key02": 1,
	  "key03": 9,
	  "key04": "1",
	  "key05": "escape",
	  "key06": "ENTER",
	  "key07": 10,
	  "key08": 1.0,
	  "key09": "invalid",
	  "key10": "",
	  "key11": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getKeyCodeVal(doc["key01"]) == sf::Keyboard::Num0);
	CHECK(Parser::getKeyCodeVal(doc["key02"]) == sf::Keyboard::Num1);
	CHECK(Parser::getKeyCodeVal(doc["key03"]) == sf::Keyboard::Num9);
	CHECK(Parser::getKeyCodeVal(doc["key04"]) == sf::Keyboard::Num1);
	CHECK(Parser::getKeyCodeVal(doc["key05"]) == sf::Keyboard::Escape);
	CHECK(Parser::getKeyCodeVal(doc["key06"]) == sf::Keyboard::Enter);
	CHECK(Parser::getKeyCodeVal(doc["key07"], sf::Keyboard::Num5) == sf::Keyboard::Num5);
	CHECK(Parser::getKeyCodeVal(doc["key08"]) == sf::Keyboard::Unknown);
	CHECK(Parser::getKeyCodeVal(doc["key09"]) == sf::Keyboard::Unknown);
	CHECK(Parser::getKeyCodeVal(doc["key10"]) == sf::Keyboard::Unknown);
	CHECK(Parser::getKeyCodeVal(doc["key11"]) == sf::Keyboard::Unknown);
}

TEST_CASE("Parser::getScanCodeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 0,
	  "key02": 1,
	  "key03": 9,
	  "key04": "1",
	  "key05": "escape",
	  "key06": "ENTER",
	  "key07": 10,
	  "key08": 1.0,
	  "key09": "invalid",
	  "key10": "",
	  "key11": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getScanCodeVal(doc["key01"]) == sf::Keyboard::Scancode::Num0);
	CHECK(Parser::getScanCodeVal(doc["key02"]) == sf::Keyboard::Scancode::Num1);
	CHECK(Parser::getScanCodeVal(doc["key03"]) == sf::Keyboard::Scancode::Num9);
	CHECK(Parser::getScanCodeVal(doc["key04"]) == sf::Keyboard::Scancode::Num1);
	CHECK(Parser::getScanCodeVal(doc["key05"]) == sf::Keyboard::Scancode::Escape);
	CHECK(Parser::getScanCodeVal(doc["key06"]) == sf::Keyboard::Scancode::Enter);
	CHECK(Parser::getScanCodeVal(doc["key07"], sf::Keyboard::Scancode::Num5) == sf::Keyboard::Scancode::Num5);
	CHECK(Parser::getScanCodeVal(doc["key08"]) == sf::Keyboard::Scancode::Unknown);
	CHECK(Parser::getScanCodeVal(doc["key09"]) == sf::Keyboard::Scancode::Unknown);
	CHECK(Parser::getScanCodeVal(doc["key10"]) == sf::Keyboard::Scancode::Unknown);
	CHECK(Parser::getScanCodeVal(doc["key11"]) == sf::Keyboard::Scancode::Unknown);
}

TEST_CASE("Parser::getTimeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": 1.2,
	  "key03": "1",
	  "key04": "10",
	  "key05": "10.6",
	  "key06": "10:20",
	  "key07": "10:20.6",
	  "key08": "1:30:05",
	  "key09": "1:30:05.6",
	  "key10": "-10",
	  "key11": "-10.6",
	  "key12": "-10:20",
	  "key13": "-10:20.6",
	  "key14": "-1:30:05",
	  "key15": "-1:30:05.6",
	  "key16": -1,
	  "key17": -1.2,
	  "key18": "invalid",
	  "key19": "",
	  "key20": true,
	  "key21": "-1:-30:-05.-6"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getTimeVal(doc["key01"]) == sf::milliseconds(10));
	CHECK(Parser::getTimeVal(doc["key02"]).asSeconds() == 1.2f);
	CHECK(Parser::getTimeVal(doc["key03"]).asSeconds() == 1.f);
	CHECK(Parser::getTimeVal(doc["key04"]).asSeconds() == 10.f);
	CHECK(Parser::getTimeVal(doc["key05"]).asSeconds() == 10.6f);
	CHECK(Parser::getTimeVal(doc["key06"]).asSeconds() == 620.f);
	CHECK(Parser::getTimeVal(doc["key07"]).asSeconds() == 620.6f);
	CHECK(Parser::getTimeVal(doc["key08"]).asSeconds() == 5405.f);
	CHECK(Parser::getTimeVal(doc["key09"]).asSeconds() == 5405.6f);
	CHECK(Parser::getTimeVal(doc["key10"]).asSeconds() == -10.f);
	CHECK(Parser::getTimeVal(doc["key11"]).asSeconds() == -10.6f);
	CHECK(Parser::getTimeVal(doc["key12"]).asSeconds() == -620.f);
	CHECK(Parser::getTimeVal(doc["key13"]).asSeconds() == -620.6f);
	CHECK(Parser::getTimeVal(doc["key14"]).asSeconds() == -5405.f);
	CHECK(Parser::getTimeVal(doc["key15"]).asSeconds() == -5405.6f);
	CHECK(Parser::getTimeVal(doc["key16"]) == sf::milliseconds(-1));
	CHECK(Parser::getTimeVal(doc["key17"]).asSeconds() == -1.2f);
	CHECK(Parser::getTimeVal(doc["key18"], sf::seconds(1.f)) == sf::seconds(1.f));
	CHECK(Parser::getTimeVal(doc["key19"]) == sf::Time::Zero);
	CHECK(Parser::getTimeVal(doc["key20"]) == sf::Time::Zero);
	CHECK(Parser::getTimeVal(doc["key21"]).asSeconds() == -5405.6f);
}

TEST_CASE("Parser::getTimeUVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 10,
	  "key02": 1.2,
	  "key03": "1",
	  "key04": "10",
	  "key05": "10.6",
	  "key06": "10:20",
	  "key07": "10:20.6",
	  "key08": "1:30:05",
	  "key09": "1:30:05.6",
	  "key10": "-10",
	  "key11": "-10.6",
	  "key12": "-10:20",
	  "key13": "-10:20.6",
	  "key14": "-1:30:05",
	  "key15": "-1:30:05.6",
	  "key16": -1,
	  "key17": -1.2,
	  "key18": "invalid",
	  "key19": "",
	  "key20": true,
	  "key21": "-1:-30:-05.-6"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getTimeUVal(doc["key01"]) == sf::milliseconds(10));
	CHECK(Parser::getTimeUVal(doc["key02"]).asSeconds() == 1.2f);
	CHECK(Parser::getTimeUVal(doc["key03"]).asSeconds() == 1.f);
	CHECK(Parser::getTimeUVal(doc["key04"]).asSeconds() == 10.f);
	CHECK(Parser::getTimeUVal(doc["key05"]).asSeconds() == 10.6f);
	CHECK(Parser::getTimeUVal(doc["key06"]).asSeconds() == 620.f);
	CHECK(Parser::getTimeUVal(doc["key07"]).asSeconds() == 620.6f);
	CHECK(Parser::getTimeUVal(doc["key08"]).asSeconds() == 5405.f);
	CHECK(Parser::getTimeUVal(doc["key09"]).asSeconds() == 5405.6f);
	CHECK(Parser::getTimeUVal(doc["key10"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key11"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key12"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key13"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key14"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key15"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key16"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key17"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key18"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key19"], sf::seconds(-1.f)) == sf::seconds(-1.f));
	CHECK(Parser::getTimeUVal(doc["key20"]) == sf::Time::Zero);
	CHECK(Parser::getTimeUVal(doc["key21"]) == sf::Time::Zero);
}

TEST_CASE("Parser::getStringVectorVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": [],
	  "key02": ["this", "is", 1, true, "string", 9.9],
	  "key03": ["text", [1, 2, 3], [1, 2, 3, 4, 5], null],
	  "key04": "text",
	  "key05": "",
	  "key06": null
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	std::vector<std::string> vec;

	CHECK(Parser::getStringVectorVal(doc["key01"]) == vec);

	vec = { "this", "is", "1", "true", "string", "9.9" };

	CHECK(Parser::getStringVectorVal(doc["key02"]) == vec);

	vec = { "text", "[1,2,3]", "[1,2,3,4,5]", "null" };

	CHECK(Parser::getStringVectorVal(doc["key03"]) == vec);

	vec = { "text" };

	CHECK(Parser::getStringVectorVal(doc["key04"]) == vec);

	vec = { "" };

	CHECK(Parser::getStringVectorVal(doc["key05"]) == vec);

	vec = {};

	CHECK(Parser::getStringVectorVal(doc["key06"]) == vec);
}

TEST_CASE("Parser::getIgnoreResourceVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "draw",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": false,
	  "key06": ["draw"],
	  "key07": ["draw", "update"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getIgnoreResourceVal(doc["key01"]) == IgnoreResource::Draw);
	CHECK(Parser::getIgnoreResourceVal(doc["key02"], IgnoreResource::All) == IgnoreResource::All);
	CHECK(Parser::getIgnoreResourceVal(doc["key03"]) == IgnoreResource::None);
	CHECK(Parser::getIgnoreResourceVal(doc["key04"]) == (IgnoreResource::Draw | IgnoreResource::Update));
	CHECK(Parser::getIgnoreResourceVal(doc["key05"]) == IgnoreResource::None);
	CHECK(Parser::getIgnoreResourceVal(doc["key06"]) == IgnoreResource::Draw);
	CHECK(Parser::getIgnoreResourceVal(doc["key07"]) == (IgnoreResource::Draw | IgnoreResource::Update));
}

TEST_CASE("Parser::getInputEventTypeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "leftClick",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": false,
	  "key06": ["leftClick"],
	  "key07": ["leftClick", "rightClick"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getInputEventTypeVal(doc["key01"]) == InputEventType::LeftClick);
	CHECK(Parser::getInputEventTypeVal(doc["key02"], InputEventType::All) == InputEventType::All);
	CHECK(Parser::getInputEventTypeVal(doc["key03"]) == InputEventType::None);
	CHECK(Parser::getInputEventTypeVal(doc["key04"]) == InputEventType::All);
	CHECK(Parser::getInputEventTypeVal(doc["key05"]) == InputEventType::None);
	CHECK(Parser::getInputEventTypeVal(doc["key06"]) == InputEventType::LeftClick);
	CHECK(Parser::getInputEventTypeVal(doc["key07"]) == (InputEventType::LeftClick | InputEventType::RightClick));
}

TEST_CASE("Parser::getMinMaxIntVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": -10,
	  "key02": 20.2,
	  "key03": 30,
	  "key04": "min",
	  "key05": "max",
	  "key06": "invalid",
	  "key07": "",
	  "key08": true,
	  "key09": false,
	  "key10": "MAX",
	  "key11": "1",
	  "key12": 9000000000
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("int")
	{
		using Type = int;

		CHECK(Parser::getMinMaxIntVal<Type>(doc["key01"]) == -10);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key02"]) == 20);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key03"]) == 30);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key04"]) == std::numeric_limits<Type>::min());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key05"]) == std::numeric_limits<Type>::max());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key06"], 1) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key07"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key08"]) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key09"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key10"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key11"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key12"]) == 0);
	}
	SECTION("int64_t")
	{
		using Type = int64_t;

		CHECK(Parser::getMinMaxIntVal<Type>(doc["key01"]) == -10);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key02"]) == 20);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key03"]) == 30);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key04"]) == std::numeric_limits<Type>::min());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key05"]) == std::numeric_limits<Type>::max());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key06"], 1) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key07"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key08"]) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key09"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key10"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key11"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key12"]) == 0);
	}
	SECTION("uint32_t")
	{
		using Type = uint32_t;

		CHECK(Parser::getMinMaxIntVal<Type>(doc["key01"]) > 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key02"]) == 20);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key03"]) == 30);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key04"]) == std::numeric_limits<Type>::min());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key05"]) == std::numeric_limits<Type>::max());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key06"], 1) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key07"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key08"]) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key09"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key10"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key11"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key12"]) == 0);
	}
	SECTION("float")
	{
		using Type = float;

		CHECK(Parser::getMinMaxIntVal<Type>(doc["key01"]) == -10.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key02"]) == 20.2f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key03"]) == 30.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key04"]) == std::numeric_limits<Type>::min());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key05"]) == std::numeric_limits<Type>::max());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key06"], 1.f) == 1.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key07"]) == 0.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key08"]) == 1.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key09"]) == 0.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key10"]) == 0.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key11"]) == 0.f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key12"]) == 0.f);
	}
	SECTION("Number32")
	{
		using Type = Number32;

		CHECK(Parser::getMinMaxIntVal<Type>(doc["key01"]) == -10);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key02"]) == 20.2f);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key03"]) == 30u);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key04"]) == std::numeric_limits<Type>::min());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key05"]) == std::numeric_limits<Type>::max());
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key06"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key07"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key08"]) == 1);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key09"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key10"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key11"]) == 0);
		CHECK(Parser::getMinMaxIntVal<Type>(doc["key12"]) == 0);
	}
	SECTION("getMinMaxNumber32Val")
	{
		CHECK(Parser::getMinMaxNumber32Val(doc["key01"]) == -10);
		CHECK(Parser::getMinMaxNumber32Val(doc["key02"]) == 20.2f);
		CHECK(Parser::getMinMaxNumber32Val(doc["key03"]) == 30u);
		CHECK(Parser::getMinMaxNumber32Val(doc["key04"]) == std::numeric_limits<int32_t>::min());
		CHECK(Parser::getMinMaxNumber32Val(doc["key05"]) == std::numeric_limits<int32_t>::max());
		CHECK(Parser::getMinMaxNumber32Val(doc["key06"]) == 0);
		CHECK(Parser::getMinMaxNumber32Val(doc["key07"]) == 0);
		CHECK(Parser::getMinMaxNumber32Val(doc["key08"]) == 1);
		CHECK(Parser::getMinMaxNumber32Val(doc["key09"]) == 0);
		CHECK(Parser::getMinMaxNumber32Val(doc["key10"]) == 0);
		CHECK(Parser::getMinMaxNumber32Val(doc["key11"]) == 0);
		CHECK(Parser::getMinMaxNumber32Val(doc["key12"]) == 0);
	}
}

TEST_CASE("Parser::getPrimitiveTypeVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "points",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getPrimitiveTypeVal(doc["key01"]) == sf::PrimitiveType::Points);
	CHECK(Parser::getPrimitiveTypeVal(doc["key02"], sf::PrimitiveType::Points) == sf::PrimitiveType::Points);
	CHECK(Parser::getPrimitiveTypeVal(doc["key03"]) == sf::PrimitiveType::TriangleStrip);
	CHECK(Parser::getPrimitiveTypeVal(doc["key04"]) == sf::PrimitiveType::TriangleStrip);
}

// Parser::getQueryVal == JsonUtils::query
TEST_CASE("Parser::getQueryVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "document": {
	    "key01": {
	      "subKey01": 1234
	    }
	  },
	  "query01": "/key01/subKey01",
	  "query02": "invalid"
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("valid")
	{
		CHECK(Parser::getQueryVal(doc["document"], doc["query01"]) == doc["document"]["key01"]["subKey01"]);
	}
	SECTION("invalid")
	{
		CHECK(Parser::getQueryVal(doc["document"], doc["query02"]) == doc["query02"]);
	}
}

TEST_CASE("Parser::getReplaceVarsVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": "value",
	  "key02": "invalid",
	  "key03": "",
	  "key04": true,
	  "key05": false,
	  "key06": ["value"],
	  "key07": ["value", "string"]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	CHECK(Parser::getReplaceVarsVal(doc["key01"]) == ReplaceVars::Value);
	CHECK(Parser::getReplaceVarsVal(doc["key02"], ReplaceVars::Value) == ReplaceVars::Value);
	CHECK(Parser::getReplaceVarsVal(doc["key03"]) == ReplaceVars::None);
	CHECK(Parser::getReplaceVarsVal(doc["key04"]) == ReplaceVars::String);
	CHECK(Parser::getReplaceVarsVal(doc["key05"], ReplaceVars::Value) == ReplaceVars::None);
	CHECK(Parser::getReplaceVarsVal(doc["key06"]) == ReplaceVars::None);
	CHECK(Parser::getReplaceVarsVal(doc["key07"]) == ReplaceVars::None);
}

TEST_CASE("Parser::getVariableVal", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "int":          1234,
	  "double":       5.678,
	  "string":       "text!",
	  "bool":         true,
	  "Int64Tuple2":  [-2, 3],
	  "Int64Tuple3":  [-2, 3, 1200],
	  "Int64Tuple4":  [-2, 3, 1200, -1298],
	  "DoubleTuple2": [-2.1, 3.9],
	  "DoubleTuple3": [-2.1, 3.9, 4.0],
	  "DoubleTuple4": [-2.1, 3.9, 4.0, 1200.87],
	  "MixedTuple2":  [-2.1, 10],
	  "MixedTuple3":  [10, -2.1, -2.5],
	  "MixedTuple4":  [10, -2.1, -2.5, 1]
	})"
	};

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("bool")
	{
		Variable var;

		CHECK(Parser::getVariableVal(doc["int"], var) == true);
		CHECK((std::holds_alternative<int64_t>(var) && std::get<int64_t>(var) == 1234));

		CHECK(Parser::getVariableVal(doc["double"], var) == true);
		CHECK((std::holds_alternative<double>(var) && std::get<double>(var) == 5.678));

		CHECK(Parser::getVariableVal(doc["string"], var) == true);
		CHECK((std::holds_alternative<std::string>(var) && std::get<std::string>(var) == "text!"));

		CHECK(Parser::getVariableVal(doc["bool"], var) == true);
		CHECK((std::holds_alternative<bool>(var) && std::get<bool>(var) == true));

		CHECK(Parser::getVariableVal(doc["Int64Tuple2"], var) == true);
		CHECK((std::holds_alternative<Int64Tuple2>(var) && std::get<Int64Tuple2>(var) == Int64Tuple2(-2, 3)));

		CHECK(Parser::getVariableVal(doc["Int64Tuple3"], var) == true);
		CHECK((std::holds_alternative<Int64Tuple3>(var) && std::get<Int64Tuple3>(var) == Int64Tuple3(-2, 3, 1200)));

		CHECK(Parser::getVariableVal(doc["Int64Tuple4"], var) == true);
		CHECK((std::holds_alternative<Int64Tuple4>(var) && std::get<Int64Tuple4>(var) == Int64Tuple4(-2, 3, 1200, -1298)));

		CHECK(Parser::getVariableVal(doc["DoubleTuple2"], var) == true);
		CHECK((std::holds_alternative<DoubleTuple2>(var) && std::get<DoubleTuple2>(var) == DoubleTuple2(-2.1, 3.9)));

		CHECK(Parser::getVariableVal(doc["DoubleTuple3"], var) == true);
		CHECK((std::holds_alternative<DoubleTuple3>(var) && std::get<DoubleTuple3>(var) == DoubleTuple3(-2.1, 3.9, 4.0)));

		CHECK(Parser::getVariableVal(doc["DoubleTuple4"], var) == true);
		CHECK((std::holds_alternative<DoubleTuple4>(var) && std::get<DoubleTuple4>(var) == DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)));

		CHECK(Parser::getVariableVal(doc["MixedTuple2"], var) == false);
		CHECK(Parser::getVariableVal(doc["MixedTuple3"], var) == false);
		CHECK(Parser::getVariableVal(doc["MixedTuple4"], var) == false);
	}
	SECTION("Variable")
	{
		CHECK(Parser::getVariableVal(doc["int"]) == Variable(1234));
		CHECK(Parser::getVariableVal(doc["MixedTuple2"]) == Variable());
	}
}

TEST_CASE("Parser::getVariables", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "int":          1234,
	  "invalid":      [-2.1, 10],
	  "DoubleTuple4": [-2.1, 3.9, 4.0, 1200.87]
	}
	)" };

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("valid")
	{
		auto vars = Parser::getVariables(doc);

		CHECK(vars.size() == 2);

		CHECK(vars[0].first == "int");
		CHECK(std::holds_alternative<int64_t>(vars[0].second));
		CHECK(std::get<int64_t>(vars[0].second) == 1234);

		CHECK(vars[1].first == "DoubleTuple4");
		CHECK(std::holds_alternative<DoubleTuple4>(vars[1].second));
		CHECK(std::get<DoubleTuple4>(vars[1].second) == DoubleTuple4(-2.1, 3.9, 4.0, 1200.87));
	}
	SECTION("invalid")
	{
		CHECK(Parser::getVariablesMap(doc["int"]).empty() == true);
		CHECK(Parser::getVariablesMap(doc["DoubleTuple4"]).empty() == true);
	}
}

TEST_CASE("Parser::getVariablesMap", "Parser")
{
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "int":          1234,
	  "invalid":      [-2.1, 10],
	  "DoubleTuple4": [-2.1, 3.9, 4.0, 1200.87]
	}
	)" };

	CHECK(JsonUtils::loadJson(json, doc) == true);

	SECTION("valid")
	{
		auto vars = Parser::getVariablesMap(doc);

		CHECK(vars.size() == 2);

		CHECK(std::holds_alternative<int64_t>(vars["int"]));
		CHECK(std::get<int64_t>(vars["int"]) == 1234);

		CHECK(std::holds_alternative<DoubleTuple4>(vars["DoubleTuple4"]));
		CHECK(std::get<DoubleTuple4>(vars["DoubleTuple4"]) == DoubleTuple4(-2.1, 3.9, 4.0, 1200.87));
	}
	SECTION("invalid")
	{
		CHECK(Parser::getVariablesMap(doc["int"]).empty() == true);
		CHECK(Parser::getVariablesMap(doc["DoubleTuple4"]).empty() == true);
	}
}

TEST_CASE("Parser::getVarOrPredicateVal", "Parser")
{
	Game game;
	rapidjson::Document doc;

	std::string json{ R"(
	{
	  "key01": 1234,
	  "key02": { "name": "file.exists", "file": "text.txt" },
	  "key03": [-2.1, 10]
	}
	)" };

	CHECK(JsonUtils::loadJson(json, doc) == true);

	auto var = Parser::getVarOrPredicateVal(game, doc["key01"]);

	CHECK(std::holds_alternative<Variable>(var));
	CHECK(std::get<Variable>(var) == Variable(1234));

	var = Parser::getVarOrPredicateVal(game, doc["key02"]);

	CHECK(holdsPredicate(var) == true);
	CHECK(std::get<std::shared_ptr<Predicate>>(var) != nullptr);

	var = Parser::getVarOrPredicateVal(game, doc["key03"]);

	CHECK(std::holds_alternative<Variable>(var));
	CHECK(std::get<Variable>(var) == Variable());
}
