#pragma once

#include "Json/JsonParser.h"
#include "Parser/ParserProperties.h"
#include "ParseUtilsIdx.h"
#include "ParseUtilsKey.h"
#include "ParseUtilsVal.h"
#include <string>
#include <string_view>

namespace Parser
{
	ReplaceVars getReplaceVars(const std::string_view str, ReplaceVars val = ReplaceVars::None);

	bool getIdFromFile(const std::string_view file, std::string& id);

	bool isValidArray(const rapidjson::Value& elem, const std::string_view key);

	bool isValidId(const std::string_view id) noexcept;

	bool isValidString(const rapidjson::Value& elem);

	bool isValidString(const rapidjson::Value& elem, const std::string_view key);
}
