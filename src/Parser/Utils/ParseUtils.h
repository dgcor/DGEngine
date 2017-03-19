#pragma once

#include "Game.h"
#include "Json/JsonParser.h"
#include "ParseUtilsIdx.h"
#include "ParseUtilsKey.h"
#include "ParseUtilsVal.h"
#include <string>

namespace Parser
{
	bool getIdFromFile(const std::string& file, std::string& id);

	bool isValidArray(const rapidjson::Value& elem, const char* key);

	bool isValidId(const std::string& id);

	bool isValidString(const rapidjson::Value& elem, const char* key);

	void replaceValWithString(rapidjson::Value& value,
		rapidjson::Value::AllocatorType& allocator,
		const std::string& oldStr, const std::string& newStr);

	void replaceValWithQueryable(rapidjson::Value& value,
		rapidjson::Value::AllocatorType& allocator, const Queryable& obj);

	void replaceValWithGameVar(rapidjson::Value& value,
		rapidjson::Value::AllocatorType& allocator, const Game& obj);
}
