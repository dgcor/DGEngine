#include "catch2/catch_test_macros.hpp"

#include "Game/Utils/FileUtils.h"
#include "Json/JsonUtils.h"
#include "Json/SaveUtils.h"
#include "Parser/Utils/ParseUtilsKey.h"

TEST_CASE("SaveUtils::hasProperty", "SaveUtils")
{
	UnorderedStringMap<Variable> map =
	{
		{ std::string("key01"), Variable(1234) }
	};

	CHECK(SaveUtils::hasProperty(map, "key01") == true);
	CHECK(SaveUtils::hasProperty(map, "key02") == false);
}

TEST_CASE("SaveUtils::getProperty", "SaveUtils")
{
	UnorderedStringMap<Variable> map =
	{
		{ std::string("key01"), Variable(0) },
		{ std::string("key02"), Variable(1234) },
		{ std::string("key03"), Variable(0.0) },
		{ std::string("key04"), Variable(5.678) },
		{ std::string("key05"), Variable("") },
		{ std::string("key06"), Variable("text!") },
		{ std::string("key07"), Variable(false) },
		{ std::string("key08"), Variable(true) },
		{ std::string("key09"), Variable(Int64Tuple2(-2, 3)) },
		{ std::string("key10"), Variable(Int64Tuple3(-2, 3, 1200)) },
		{ std::string("key11"), Variable(Int64Tuple4(-2, 3, 1200, -1298)) },
		{ std::string("key12"), Variable(DoubleTuple2(-2.1, 3.9)) },
		{ std::string("key13"), Variable(DoubleTuple3(-2.1, 3.9, 4.0)) },
		{ std::string("key14"), Variable(DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)) }
	};

	SECTION("getBoolProperty")
	{
		CHECK(SaveUtils::getBoolProperty(map, "key01") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key02") == true);
		CHECK(SaveUtils::getBoolProperty(map, "key03") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key04") == true);
		CHECK(SaveUtils::getBoolProperty(map, "key05") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key06") == true);
		CHECK(SaveUtils::getBoolProperty(map, "key07") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key08") == true);
		CHECK(SaveUtils::getBoolProperty(map, "key09") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key10") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key11") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key12") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key13") == false);
		CHECK(SaveUtils::getBoolProperty(map, "key14") == false);
	}
	SECTION("getInt64Property")
	{
		CHECK(SaveUtils::getInt64Property(map, "key01") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key02") == 1234);
		CHECK(SaveUtils::getInt64Property(map, "key03") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key04") == 5);
		CHECK(SaveUtils::getInt64Property(map, "key05") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key06") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key07") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key08") == 1);
		CHECK(SaveUtils::getInt64Property(map, "key09") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key10") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key11") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key12") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key13") == 0);
		CHECK(SaveUtils::getInt64Property(map, "key14") == 0);
	}
	SECTION("getDoubleProperty")
	{
		CHECK(SaveUtils::getDoubleProperty(map, "key01") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key02") == 1234.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key03") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key04") == 5.678);
		CHECK(SaveUtils::getDoubleProperty(map, "key05") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key06") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key07") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key08") == 1.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key09") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key10") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key11") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key12") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key13") == 0.0);
		CHECK(SaveUtils::getDoubleProperty(map, "key14") == 0.0);
	}
	SECTION("getStringProperty")
	{
		CHECK(SaveUtils::getStringProperty(map, "key01") == "0");
		CHECK(SaveUtils::getStringProperty(map, "key02") == "1234");
		CHECK(SaveUtils::getStringProperty(map, "key03") == "0.0");
		CHECK(SaveUtils::getStringProperty(map, "key04") == "5.678");
		CHECK(SaveUtils::getStringProperty(map, "key05") == "");
		CHECK(SaveUtils::getStringProperty(map, "key06") == "text!");
		CHECK(SaveUtils::getStringProperty(map, "key07") == "false");
		CHECK(SaveUtils::getStringProperty(map, "key08") == "true");
		CHECK(SaveUtils::getStringProperty(map, "key09") == "[-2, 3]");
		CHECK(SaveUtils::getStringProperty(map, "key10") == "[-2, 3, 1200]");
		CHECK(SaveUtils::getStringProperty(map, "key11") == "[-2, 3, 1200, -1298]");
		CHECK(SaveUtils::getStringProperty(map, "key12") == "[-2.1, 3.9]");
		CHECK(SaveUtils::getStringProperty(map, "key13") == "[-2.1, 3.9, 4.0]");
		CHECK(SaveUtils::getStringProperty(map, "key14") == "[-2.1, 3.9, 4.0, 1200.87]");
	}
}

TEST_CASE("SaveUtils::write", "SaveUtils")
{
	using namespace rapidjson;
	using namespace SaveUtils;

	StringBuffer buffer(0, std::numeric_limits<uint16_t>::max());
	PrettyWriter<StringBuffer> writer(buffer);
	writer.SetIndent(' ', 2);

	// root
	writer.StartObject();

	writeBool(writer,       "key01", true);
	writeInt(writer,        "key02", -1200);
	writeInt64(writer,      "key03", -90000000000);
	writeUInt(writer,       "key04", 1200);
	writeUInt64(writer,     "key05", 90000000000);
	writeDouble(writer,     "key06", 2.5);
	writeNumber32(writer,   "key07", Number32(1.5f));
	writeNumber32(writer,   "key08", Number32(-2));
	writeNumber32(writer,   "key09", Number32(2u));
	writeVector2i(writer,   "key10", sf::Vector2i(-2, 5));
	writeVector2f(writer,   "key11", sf::Vector2f(-2.f, 5.f));
	writeVector2f(writer,   "key12", sf::Vector2f(-2.5f, 5.5f));
	writeVector2fi(writer,  "key13", sf::Vector2f(-2.f, 5.f));
	writeVector2fi(writer,  "key14", sf::Vector2f(-2.5f, 5.5f));
	writeString(writer,     "key15", "text");
	writeStringView(writer, "key16", "text");
	writeVariable(writer,   "key17", Variable(0));
	writeVariable(writer,   "key18", Variable(1234));
	writeVariable(writer,   "key19", Variable(0.0));
	writeVariable(writer,   "key20", Variable(5.678));
	writeVariable(writer,   "key21", Variable(""));
	writeVariable(writer,   "key22", Variable("text!"));
	writeVariable(writer,   "key23", Variable(true));
	writeVariable(writer,   "key24", Variable(Int64Tuple2(-2, 3)));
	writeVariable(writer,   "key25", Variable(Int64Tuple3(-2, 3, 1200)));
	writeVariable(writer,   "key26", Variable(Int64Tuple4(-2, 3, 1200, -1298)));
	writeVariable(writer,   "key27", Variable(DoubleTuple2(-2.1, 3.9)));
	writeVariable(writer,   "key28", Variable(DoubleTuple3(-2.1, 3.9, 4.0)));
	writeVariable(writer,   "key29", Variable(DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)));

	// root
	writer.EndObject();

	Document doc;

	std::string_view json{ buffer.GetString(), buffer.GetSize() };

	CHECK(FileUtils::readText("json/saveFile.json") == json);
	CHECK(JsonUtils::loadJson(json, doc) == true);

	using v2i = sf::Vector2i;
	using v2f = sf::Vector2f;

	CHECK(Parser::getBoolKey(doc,          "key01") == true);
	CHECK(Parser::getIntKey(doc,           "key02") == -1200);
	CHECK(Parser::getInt64Key(doc,         "key03") == -90000000000);
	CHECK(Parser::getUIntKey(doc,          "key04") == 1200);
	CHECK(Parser::getUInt64Key(doc,        "key05") == 90000000000);
	CHECK(Parser::getDoubleKey(doc,        "key06") == 2.5);
	CHECK(Parser::getDoubleKey(doc,        "key07") == 1.5f);
	CHECK(Parser::getIntKey(doc,           "key08") == -2);
	CHECK(Parser::getUIntKey(doc,          "key09") == 2u);
	CHECK(Parser::getVector2iKey<v2i>(doc, "key10") == v2i(-2, 5));
	CHECK(Parser::getVector2fKey<v2f>(doc, "key11") == v2f(-2.f, 5.f));
	CHECK(Parser::getVector2fKey<v2f>(doc, "key12") == v2f(-2.5f, 5.5f));
	CHECK(Parser::getVector2fKey<v2f>(doc, "key13") == v2f(-2.f, 5.f));
	CHECK(Parser::getVector2fKey<v2f>(doc, "key14") == v2f(-2.5f, 5.5f));
	CHECK(Parser::getStringViewKey(doc,    "key15") == "text");
	CHECK(Parser::getStringViewKey(doc,    "key16") == "text");
	CHECK(Parser::getVariableKey(doc,      "key17") == Variable(0));
	CHECK(Parser::getVariableKey(doc,      "key18") == Variable(1234));
	CHECK(Parser::getVariableKey(doc,      "key19") == Variable(0.0));
	CHECK(Parser::getVariableKey(doc,      "key20") == Variable(5.678));
	CHECK(Parser::getVariableKey(doc,      "key21") == Variable(""));
	CHECK(Parser::getVariableKey(doc,      "key22") == Variable("text!"));
	CHECK(Parser::getVariableKey(doc,      "key23") == Variable(true));
	CHECK(Parser::getVariableKey(doc,      "key24") == Variable(Int64Tuple2(-2, 3)));
	CHECK(Parser::getVariableKey(doc,      "key25") == Variable(Int64Tuple3(-2, 3, 1200)));
	CHECK(Parser::getVariableKey(doc,      "key26") == Variable(Int64Tuple4(-2, 3, 1200, -1298)));
	CHECK(Parser::getVariableKey(doc,      "key27") == Variable(DoubleTuple2(-2.1, 3.9)));
	CHECK(Parser::getVariableKey(doc,      "key28") == Variable(DoubleTuple3(-2.1, 3.9, 4.0)));
	CHECK(Parser::getVariableKey(doc,      "key29") == Variable(DoubleTuple4(-2.1, 3.9, 4.0, 1200.87)));
}
