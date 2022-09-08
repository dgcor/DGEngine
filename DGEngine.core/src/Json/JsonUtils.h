#pragma once

#include <functional>
#include "Game/Queryable.h"
#include "Game/Variable.h"
#include "JsonParser.h"
#include <string_view>

class Game;

namespace JsonUtils
{
	// query json document using JSON pointer syntax
	// returns query object if no match
	// extends JSON pointer to support querying arrays by key = value
	// ex: "/layers/0/name"                  -> background
	//     "/layers/{name=collision}/name"   -> collision
	const rapidjson::Value& query(const rapidjson::Value& elem, const rapidjson::Value& query);

	// replaces string value occurrences of "%str%" with obj.getProperty("str")
	void replaceStringValueWithQueryable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const Queryable& obj, bool changeValueType = false);

	// replaces string value with variable
	void replaceStringValueWithVariable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const std::string_view str, const Variable& var, bool changeValueType = false);

	// replaces value with variable
	void replaceValueWithVariable(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator, const Variable& var);

	// replaces all string values with function
	void replaceValuesWithFunction(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const std::function<void(rapidjson::Value&)> stringValueReplaceFunc);

	// replaces all string values with string
	void replaceValuesWithString(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const std::string_view oldStr, const std::string_view newStr);

	// replaces all string value occurrences of "%str%" with game.getVarOrProp("str")
	void replaceValuesWithGameVar(rapidjson::Value& elem,
		rapidjson::Value::AllocatorType& allocator,
		const Game& obj, bool changeValueType, char token = '%');

	std::string toString(const rapidjson::Value& elem);
	std::string jsonToString(const rapidjson::Value& elem);
	std::string jsonToPrettyString(const rapidjson::Value& elem);

	// loads json from a file
	bool loadFile(const std::string_view file, rapidjson::Document& doc);

	// loads json from a json string
	bool loadJson(const std::string_view json, rapidjson::Document& doc);

	// loads json from a json string and replaces all string value occurrences of "%str%" with game.getVarOrProp("str")
	bool loadJsonAndReplaceValues(const std::string_view json, rapidjson::Document& doc,
		const Game& obj, bool changeValueType, char token = '%');

	void saveToFile(const std::string_view file, const rapidjson::Value& elem);

	void saveToFile(const std::string_view file, const char* key,
		const std::vector<std::pair<std::string, Variable>>& container);
}
