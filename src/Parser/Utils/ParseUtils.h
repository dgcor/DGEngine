#pragma once

#include "Json/JsonParser.h"
#include "ParseUtilsIdx.h"
#include "ParseUtilsKey.h"
#include "ParseUtilsVal.h"
#include "ParseUtilsVar.h"
#include <string>

namespace Parser
{
	bool getIdFromFile(const std::string& file, std::string& id);

	bool isValidArray(const rapidjson::Value& elem, const char* key);

	bool isValidId(const std::string& id);

	bool isValidString(const rapidjson::Value& elem, const char* key);
}
