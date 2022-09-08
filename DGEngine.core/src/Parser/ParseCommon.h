#pragma once

#include "Json/JsonParser.h"
#include <string_view>

namespace Parser
{
	// tries to get id from id element
	std::string_view parseValidId(const rapidjson::Value& elem);

	// tries to get id from id element. if id is not set, tries to get id from file element (name)
	std::string parseValidIdOrFile(const rapidjson::Value& elem, const std::string_view fileKey = "file");

	// tries to get id from id element. if id is not set, tries to get id from file (name)
	std::string parseValidIdOrFilePath(const rapidjson::Value& elem, const std::string_view filePath);
}
